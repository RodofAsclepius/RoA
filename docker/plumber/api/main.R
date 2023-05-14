##############################################################################
usePackage = function(p)
{
  if (!is.element(p, installed.packages()[,1]))
    install.packages(p, lib="/usr/local/lib/R/site-library", dep = TRUE);
  require(p, character.only = TRUE)
}

################################################################################
init = function ()
{
  usePackage("jsonlite") # parsing
  usePackage('RPostgreSQL') # database
  usePackage('useful') # build.formale
  usePackage('cobalt')
  usePackage('e1071') #kurtosis
  usePackage('survey') # survey effect esimates
  usePackage('dagitty') # adjustment sets
  usePackage("plotrix") #wtd.hist
  usePackage('weights') # wtd.hist
  usePackage('dplyr')
  usePackage('MXM') # compute causal models
  usePackage('purrr')
  
  #usePackage('drtmle') # wald_test
  

  # turn of graphics
  graphics.off()
  par("mar")
  par(mar=c(1,1,1,1))

  # setup environment
  maindataframe = data.frame()
  maindataframetypes = data.frame()

  assign("main", maindataframe, .GlobalEnv)
  assign("variablestypes", maindataframetypes, .GlobalEnv)

  # Set default unboxed json serializer
  #setSerializer(plumber::serializer_unboxed_json())

  print('R server initialized for Asclepius')

}
init()
print(sessionInfo())

###############################################################################
#* @serializer unboxedJSON
#* @post /resetworkspace
resetWorkspace = function()
{
  #print("************************** resetWorkspace **************************")

  rm(list = ls())
  init()

  #print("************************** end resetWorkspace **************************")
}

###############################################################################
#* @serializer unboxedJSON
#* @post /ping
ping = function()
{
  #print("ping -> pong")
  return ("pong")
}

###############################################################################
#* @serializer unboxedJSON
#* @post /connectToDatabase
connectToDatabase = function(thedatabasename, username, password, maintablename, variablestypestablename)
{
  print("**** connectDatabase ****")
  
  dbconfig = list()

  print(paste("Connecting to ", thedatabasename, "@ localhost:5432"))

  dbconfig["databasename"] = thedatabasename
  dbconfig["username"] = username
  dbconfig["password"] = password
  dbconfig["maintablename"] = maintablename
  dbconfig["variablestypestablename"] = variablestypestablename

  assign("dbconfig", dbconfig, .GlobalEnv)

  res = dbCanConnect(dbDriver("PostgreSQL"), dbname = dbconfig["databasename"],
                     host = "localhost", port = 5432,
                     user = dbconfig["username"], password = dbconfig["password"])

  result = list()
  result["dbcanconnect"] = res

  if(res)
  {
    print("Succes...")
  }
  else {
    print("Failed...")
  }

  prepareDataFrames()

  print("**** connectDatabase done ****")

  return(result)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /getVariables
getVariables = function()
{
  print("getVariables")
  prepareDataFrames()
  

  variables = list()

  tryCatch({
    #prepareDataFrames()

      # todo: fix empty table issue
    maindataframe <- eval(as.name("main"), .GlobalEnv)
    maindataframetypes <- eval(as.name("variablestypes"), .GlobalEnv)
    variableLevels = sapply(maindataframe, levels)

    for(i in 1:nrow(maindataframetypes))
    {
      variable = list()
      variable["name"] = maindataframetypes[i, 1]
      variable["type"] = maindataframetypes[i, 2]
      variable[["levels"]] = unlist(variableLevels[eval(paste(variable$name))])
      variableProperties = list()
      variable[["properties"]] = variableProperties

      variables[[length(variables) + 1]] = variable
    }

  }, warning = function(w) {
    print(w)
  }, error = function(e) {
    print(e)
  }, finally = {
  })

  result = list()
  result[["variablestypes"]] = variables

  print("getVariables done")
  
  return(result)
}

################################################################################
prepareDataFrames = function()
{
  # prepare maintable and variablestypetables
  tryCatch({

    dbconfig = eval(as.name("dbconfig"), .GlobalEnv)

  # print(dbconfig)

    con = dbConnect(dbDriver("PostgreSQL"), dbname = dbconfig["databasename"],
                    host = "localhost", port = 5432,
                    user = dbconfig["username"], password = dbconfig["password"])

    maindataframe <- dbGetQuery(con, paste("SELECT * FROM ", dbconfig["maintablename"]));
    
    maindfrows = nrow(maindataframe)
    print(maindfrows)
    # if(maindfrows > 200)
    # {
      #print('sampling, because > 500 rows')
      
      # maindataframe = sample_n(maindataframe, 350)  
      #maindataframe = sample_n(maindataframe, 500)  
    # }
    maindataframetypes <- dbGetQuery(con, paste("SELECT * FROM ", dbconfig["variablestypestablename"]));

    for(i in 1:nrow(maindataframetypes) )
    {
      variablename = maindataframetypes[i, "variable"]
      type = maindataframetypes[i, "type"]

      if(type == "integer")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.integer(maindataframe[[variablename]])

          #maindataframe[is.na(maindataframe[, variablename]), variablename] = median(maindataframe[, variablename], na.rm = TRUE)

        }, error=function(i) {
          print("error in integer variable type setting")
          print(variablename)
          print(type)
          print(i)
        }

        )
      }

      if(type == "numeric")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.numeric(maindataframe[[variablename]])

          #maindataframe[is.na(maindataframe[, variablename]), variablename] = median(maindataframe[, variablename], na.rm = TRUE)

        }, error=function(i) {
          print("error in numeric variable type setting")
          print(variablename)
          print(type)
          print(i)
        }

        )
      }

      if(type == "factor")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          #maindataframe[[variablename]] = as.numeric(factor(maindataframe[[variablename]], ordered=FALSE))
          maindataframe[[variablename]] = factor(maindataframe[[variablename]], ordered=FALSE)

          if(sum(is.na(maindataframe[[variablename]])) > 0)
          {
            # Get levels and add "None"
            levels = levels(maindataframe[, variablename])
            #levels[length(levels) + 1] = -1

            # refactor Species to include "None" as a factor level
            # and replace NA with "None"
            maindataframe[, variablename] = factor(maindataframe[, variablename], levels = levels)
            #maindataframe[, variablename][is.na(maindataframe[, variablename])] = -1
          }
        }, error=function(i) {
          print("error in factor variable type setting")
          print(variablename)
          print(type)
          print(i)
        })
      }

      if(type == "string")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.numeric(factor(maindataframe[[variablename]], ordered=FALSE))
          #maindataframe[[variablename]] = factor(maindataframe[[variablename]], ordered=FALSE)

          if(sum(is.na(maindataframe[[variablename]])) > 0)
          {
            # Get levels and add "None"
            levels = levels(maindataframe[, variablename])
            #levels[length(levels) + 1] = -1

            # refactor Species to include "None" as a factor level
            # and replace NA with "None"
            maindataframe[, variablename] = factor(maindataframe[, variablename], levels = levels)
            #maindataframe[, variablename][is.na(maindataframe[, variablename])] = -1
          }
        }, error=function(ifr) {
          print("error in factor variable type setting")
          print(variablename)
          print(type)
          print(i)
        })
      }

    }
  }, warning = function(w) {
    print(w)
  }, error = function(e) {
    print(e)
  }, finally = {
    # assign id to main table
    maindataframe$ID <- seq_along(maindataframe[,1])
    #print(maindataframe)
    assign("main", maindataframe, .GlobalEnv)
    assign("variablestypes", maindataframetypes, .GlobalEnv)
    dbDisconnect(con)
  })
}


################################################################################
sampleDataFrames = function(samplesize)
{
  # prepare maintable and variablestypetables
  tryCatch({

    dbconfig = eval(as.name("dbconfig"), .GlobalEnv)

  # print(dbconfig)

    con = dbConnect(dbDriver("PostgreSQL"), dbname = dbconfig["databasename"],
                    host = "localhost", port = 5432,
                    user = dbconfig["username"], password = dbconfig["password"])

    maindataframe <- dbGetQuery(con, paste("SELECT * FROM ", dbconfig["maintablename"]));
    
    maindfrows = nrow(maindataframe)
    print(maindfrows)
    maindataframe = sample_n(maindataframe, samplesize)  

    maindataframetypes <- dbGetQuery(con, paste("SELECT * FROM ", dbconfig["variablestypestablename"]));

    for(i in 1:nrow(maindataframetypes) )
    {
      variablename = maindataframetypes[i, "variable"]
      type = maindataframetypes[i, "type"]

      if(type == "integer")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.integer(maindataframe[[variablename]])

          #maindataframe[is.na(maindataframe[, variablename]), variablename] = median(maindataframe[, variablename], na.rm = TRUE)

        }, error=function(i) {
          print("error in integer variable type setting")
          print(variablename)
          print(type)
          print(i)
        }

        )
      }

      if(type == "numeric")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.numeric(maindataframe[[variablename]])

          #maindataframe[is.na(maindataframe[, variablename]), variablename] = median(maindataframe[, variablename], na.rm = TRUE)

        }, error=function(i) {
          print("error in numeric variable type setting")
          print(variablename)
          print(type)
          print(i)
        }

        )
      }

      if(type == "factor")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          #maindataframe[[variablename]] = as.numeric(factor(maindataframe[[variablename]], ordered=FALSE))
          maindataframe[[variablename]] = factor(maindataframe[[variablename]], ordered=FALSE)

          if(sum(is.na(maindataframe[[variablename]])) > 0)
          {
            # Get levels and add "None"
            levels = levels(maindataframe[, variablename])
            #levels[length(levels) + 1] = -1

            # refactor Species to include "None" as a factor level
            # and replace NA with "None"
            maindataframe[, variablename] = factor(maindataframe[, variablename], levels = levels)
            #maindataframe[, variablename][is.na(maindataframe[, variablename])] = -1
          }
        }, error=function(i) {
          print("error in factor variable type setting")
          print(variablename)
          print(type)
          print(i)
        })
      }

      if(type == "string")
      {
        tryCatch({
          # Remove ","
          maindataframe[[variablename]] = gsub(",", "", maindataframe[[variablename]])
          maindataframe[[variablename]] = as.numeric(factor(maindataframe[[variablename]], ordered=FALSE))
          #maindataframe[[variablename]] = factor(maindataframe[[variablename]], ordered=FALSE)

          if(sum(is.na(maindataframe[[variablename]])) > 0)
          {
            # Get levels and add "None"
            levels = levels(maindataframe[, variablename])
            #levels[length(levels) + 1] = -1

            # refactor Species to include "None" as a factor level
            # and replace NA with "None"
            maindataframe[, variablename] = factor(maindataframe[, variablename], levels = levels)
            #maindataframe[, variablename][is.na(maindataframe[, variablename])] = -1
          }
        }, error=function(ifr) {
          print("error in factor variable type setting")
          print(variablename)
          print(type)
          print(i)
        })
      }

    }
  }, warning = function(w) {
    print(w)
  }, error = function(e) {
    print(e)
  }, finally = {
    # assign id to main table
    maindataframe$ID <- seq_along(maindataframe[,1])
    #print(maindataframe)
    dbDisconnect(con)
  })

  return(maindataframe)
}


################################################################################
#* @serializer unboxedJSON
#* @post /computeDistribution
computedistribution = function(distribution)
{
  #distribution = fromJSON(distribution)

  # print(distribution)

  result = list()
  #distribution = list()
  frequencies = list()
  densities = list()
  properties = list()
  
  variable = distribution$variable
  group = distribution$group
  dfGroupQuery = group$query

  maintable = eval(as.name("main"), .GlobalEnv)
  variablestypestable = eval(as.name("variablestypes"), .GlobalEnv)

  dfGroup = subset(maintable, eval(parse(text=paste(dfGroupQuery))))
  
  switch(variable$type,
         "factor"={
           bp = table((dfGroup[[variable$name]]))
           frequencies[["labels"]] = levels(dfGroup[[variable$name]])
           frequencies[["values"]] = as.vector(as.matrix(bp)[,1])
           
           properties["kurtosis"] = kurtosis(frequencies[["values"]])
           properties["skewness"] = skewness(frequencies[["values"]])
           
           #densitiesdata = density(as.numeric(dfGroup[[variable$name]]))
           #densities[['x']] = densitiesdata$x
           #densities[['y']] = densitiesdata$y
           #densities['bw'] = densitiesdata$bw
           #densities['n'] = densitiesdata$n
         },
         "numeric"={
           histdata = hist(dfGroup[[variable$name]], freq=TRUE, right=FALSE)
           frequencies [["labels"]] = histdata$breaks
           frequencies [["values"]] = histdata$counts
           
           properties["kurtosis"] = kurtosis(frequencies[["values"]])
           properties["skewness"] = skewness(frequencies[["values"]])
           
           densitiesdata = density(dfGroup[[variable$name]])
           densities[['x']] = densitiesdata$x
           densities[['y']] = densitiesdata$y
           densities['bw'] = densitiesdata$bw
           densities['n'] = densitiesdata$n
         },
         "integer"={
           power = max(0, floor(log10(max(dfGroup[[variable$name]]) - min(dfGroup[[variable$name]])) - 1))
           if(length(seq(min(dfGroup[[variable$name]]), max(dfGroup[[variable$name]]), by=10^power)) <= 20)
           {
             histdatamaindataframe = hist(dfGroup[[variable$name]], freq=TRUE, right=FALSE,
                                          breaks=seq(min(dfGroup[[variable$name]]), max(dfGroup[[variable$name]]), by=10^power))
           }
           else
           {
             histdatamaindataframe = hist(dfGroup[[variable$name]], freq=TRUE, right=FALSE, breaks=20)
           }
           
           histdata = hist(dfGroup[[variable$name]], breaks=histdatamaindataframe$breaks, freq=TRUE, right=FALSE)
           frequencies [["labels"]] = histdata$breaks
           frequencies [["values"]] = histdata$counts
           
           properties["kurtosis"] = kurtosis(frequencies[["values"]])
           properties["skewness"] = skewness(frequencies[["values"]])
           
           densitiesdata = density(dfGroup[[variable$name]])
           densities[['x']] = densitiesdata$x
           densities[['y']] = densitiesdata$y
           densities['bw'] = densitiesdata$bw
           densities['n'] = densitiesdata$n
         })
  
  distribution[["frequencies"]] = frequencies
  distribution[["densities"]] = densities

  #print(distribution)
  
  return(distribution)
}

################################################################################
#* @serializer unboxedJSON
#* @post /computeDistributions
computedistributions = function(distributions)
{
  print("computedistributions")
  prepareDataFrames()
  
  distributions = fromJSON(distributions)

  distributionsResult = list()

  for(distribution in distributions)
  {
    tryCatch({
      distributionUpdated = computedistribution(distribution)
      distributionsResult = c(distributionsResult, list(distributionUpdated))
    }, 
    error=function(e){
      print(e)
    })
  }

  # convert to vector
  vecDistributionsResult = list()
  for(distributionResult in distributionsResult)
  {
    vecDistributionsResult = c(vecDistributionsResult, list(distributionResult))
  }
  
  result = list()
  result[["distributions"]] = vecDistributionsResult
  
  #print(result)
  
  print("computedistributions done")
  
  return(result)
}

###############################################################################

computeBalanceModelDistribution = function(distribution, dataframe, doWeight)
{
  #print("computeBalanceModelDistribution")
  
  properties = list()
  frequencies = list()
  densities = list()

  tryCatch({
    switch(distribution$variable$type,
       "factor"={

         # if(doWeight)
         # {
         #   histdata = weighted.hist(dataframe[[distribution$variable$name]], dataframe[["computedweight"]], freq=TRUE)
         # }
         # else
         # {
         #   histdata = hist(dataframe[[distribution$variable$name]], freq=TRUE, right=FALSE)  
         # }

         bp = table((dataframe[[distribution$variable$name]]))
         frequencies[["labels"]] = levels(dataframe[[distribution$variable$name]])
         frequencies[["values"]] = as.vector(as.matrix(bp)[,1])
         
         properties["kurtosis"] = kurtosis(frequencies[["values"]])
         properties["skewness"] = skewness(frequencies[["values"]])
         
         # densitiesdata = density(as.numeric(dataframe[[distribution$variable$name]]))
         # densities[['x']] = densitiesdata$x
         # densities[['y']] = densitiesdata$y
         # densities['bw'] = densitiesdata$bw
         # densities['n'] = densitiesdata$n
         
         
         # if(doWeight)
         # {
         #   histdata = weighted.hist(dataframe[[distribution$variable$name]], dataframe[["computedweight"]], freq=TRUE)
         # }
         # else
         # {
         #   histdata = hist(dataframe[[distribution$variable$name]], breaks=dataframe$breaks, freq=TRUE, right=FALSE)  
         # }
       },
       "numeric"={
         if(doWeight)
         {
           histdata = weighted.hist(dataframe[[distribution$variable$name]], dataframe[["computedweight"]], freq=TRUE)
         }
         else
         {
           histdata = hist(dataframe[[distribution$variable$name]], freq=TRUE, right=FALSE)  
         }
         frequencies [["labels"]] = histdata$breaks
         frequencies [["values"]] = histdata$counts
         
         properties["kurtosis"] = kurtosis(frequencies[["values"]])
         properties["skewness"] = skewness(frequencies[["values"]])
         
         if(doWeight)
         {
           #densitiesdata = density(dataframe[[distribution$variable$name]], weights=dataframe[["computedweight"]])
           densitiesdata = density(dataframe[[distribution$variable$name]], weights=dataframe[["computedweight"]] / sum(dataframe[["computedweight"]]))
         }
         else
         {
           densitiesdata = density(dataframe[[distribution$variable$name]])
         }
         
         densities[['x']] = densitiesdata$x
         densities[['y']] = densitiesdata$y
         densities['bw'] = densitiesdata$bw
         densities['n'] = densitiesdata$n
       },
       "integer"={
         power = max(0, floor(log10(max(dataframe[[distribution$variable$name]]) - min(dataframe[[distribution$variable$name]])) - 1))
         #power = 1

         if(length(seq(min(dataframe[[distribution$variable$name]]), max(dataframe[[distribution$variable$name]]), by=10^power)) <= 20)
         {
           if(doWeight)
           {
             histdata = weighted.hist(dataframe[[distribution$variable$name]], dataframe[["computedweight"]], freq=TRUE,
                                      breaks=seq(min(dataframe[[distribution$variable$name]]), max(dataframe[[distribution$variable$name]]) ))#, by=10^power))
           }
           else
           {
             histdata = hist(dataframe[[distribution$variable$name]], freq=TRUE, right=FALSE,
                                          breaks=seq(min(dataframe[[distribution$variable$name]]), max(dataframe[[distribution$variable$name]]) ))#, by=10^power)) 
           }
         }
         else
         {
           if(doWeight)
           {
             histdata = weighted.hist(dataframe[[distribution$variable$name]], dataframe[["computedweight"]], freq=TRUE, breaks=20)#, by=10^power)
           }
           else
           {
             histdata = hist(dataframe[[distribution$variable$name]], freq=TRUE, right=FALSE, breaks=20)
           }
           
         }

         frequencies [["labels"]] = histdata$breaks
         frequencies [["values"]] = histdata$counts
         
         properties["kurtosis"] = kurtosis(frequencies[["values"]])
         properties["skewness"] = skewness(frequencies[["values"]])
         
         if(doWeight)
         {
           densitiesdata = density(dataframe[[distribution$variable$name]], weights=dataframe[["computedweight"]] / sum(dataframe[["computedweight"]]))
         }
         else
         {
           densitiesdata = density(dataframe[[distribution$variable$name]])
         }
         densities[['x']] = densitiesdata$x
         densities[['y']] = densitiesdata$y
         densities['bw'] = densitiesdata$bw
         densities['n'] = densitiesdata$n
       })

      

  }, warning = function(w) {
    print(w)
  }, error = function(e) {
    print(e)
  }, finally = {
  })

  distribution[["frequencies"]] = frequencies
  distribution[["densities"]] = densities
  distribution[["properties"]] = properties
  
  #print(distribution)
  
  return(distribution)
}

computeBalanceModelDistributions = function(distributions, dataframe, doWeight)
{
  print("computeBalanceModelDistributions")
  
  distributionsComputed = list()
  for(distribution in unname(distributions))
  {
    tryCatch({
      distributionComputed = computeBalanceModelDistribution(distribution, dataframe, doWeight)
      distributionsComputed = c(distributionsComputed, list(distributionComputed))
    }, 
    error=function(e){
      print(e)
    })
  }
  
  return(distributionsComputed);
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeBalanceModels 
computeBalanceModels = function(balanceModels)
{
  print("computeBalanceModels")
  prepareDataFrames()
  
  balanceModels = fromJSON(balanceModels)
  
  balanceModelsComputed = list()
  for(balanceModel in balanceModels)
  {
    tryCatch({
        computedBalanceModel = computeBalanceModel(balanceModel)
        #print(computedBalanceModel)
        balanceModelsComputed = c(balanceModelsComputed, list(computedBalanceModel))
      }, 
      error=function(e){
        print(e)
    })
  }
  
  result = list()
  result[["balanceModels"]] = balanceModelsComputed

  print("computeBalanceModels done")
  
  return(result)
}

computeBalanceModel = function(balanceModel)
{
  print("computeBalanceModel")

  dfGroupQuery = balanceModel$group$query
  treat = balanceModel$treatment$variable$name
  covs = unlist(lapply(balanceModel$variablesBaseline, function(x){return(x$name)}))
  estmethod = balanceModel$treatment$estimationMethod

  #covs = c("age")

  covsInGroupQuery = all.vars(parse(text=paste(dfGroupQuery)))
  nocovs = list("p.score", "ID", "computedweight", 
                unlist(covsInGroupQuery))
  
  maintable = eval(as.name("main"), .GlobalEnv)
  variablestypestable = eval(as.name("variablestypes"), .GlobalEnv)
  dfGroup = subset(maintable, eval(parse(text=paste(dfGroupQuery))))

  # Compute propensity scores and use weighting
  model = list()
  if(length(covs) > 0)
  {
    f = build.formula(treat, covs)
    
    dfGroup$p.score <- glm(f, data = dfGroup, family = "binomial")$fitted.values
    covariates <- subset(dfGroup, select = covs)

    # Propensity score weighting using IPTW
    print(paste("method:", estmethod))
    sddenomoption = "pooled"
    if(estmethod == "ATE")
    {
      dfGroup$computedweight <- ifelse(dfGroup[[treat]] == balanceModel$treatment$levelTreatment, 1/dfGroup$p.score, 1/(1-dfGroup$p.score))  
      sddenomoption = "pooled"
    }
    if(estmethod == "ATT")
    {
      dfGroup$computedweight <- ifelse(dfGroup[[treat]] == balanceModel$treatment$levelTreatment, 1, dfGroup$p.score/(1-dfGroup$p.score))
      sddenomoption = "treated"
    }


    # data frame interface:
    addcov = names(dfGroup[ , !(names(dfGroup) %in% c(covs, treat, nocovs))])
    #dfGroup[[paste(treat)]] = factor( dfGroup[[paste(treat)]] )
    model = bal.tab(covariates, treat = paste(treat), data = dfGroup,  
                    weights = "computedweight", method = "weighting", 
                    s.d.denom = paste(sddenomoption),
                    binary = "std", 
                    continuous = "std", 
                    un = TRUE, addl = addcov, disp.v.ratio = TRUE, disp.means = TRUE, disp.bal.tab = TRUE)
  }

  if(length(covs) == 0)
  {
    sddenomoption = "pooled"
    if(estmethod == "ATE")
    {
      dfGroup$computedweight <- ifelse(dfGroup[[treat]] == balanceModel$treatment$levelTreatment, 1/dfGroup$p.score, 1/(1-dfGroup$p.score))  
      sddenomoption = "pooled"
    }
    if(estmethod == "ATT")
    {
      dfGroup$computedweight <- ifelse(dfGroup[[treat]] == balanceModel$treatment$levelTreatment, 1, dfGroup$p.score/(1-dfGroup$p.score))
      sddenomoption = "treated"
    }

    # nothing to condition on, so we balance over all variables and overwrite adjusted values with unadjusted ones
    covariates = dfGroup[ , !(names(dfGroup) %in% c(nocovs))]
    
    # Hier gaat het fout
    #print(covariates)
    print(treat)
    #print(dfGroup)
    print(sddenomoption)
    model = bal.tab(covariates, treat = paste(treat), data = dfGroup, method = "weighting", 
                    s.d.denom = paste(sddenomoption), #weights = "computedweight",
                    binary = "std", 
                    continuous = "std", 
                    un = TRUE, disp.v.ratio = TRUE, disp.means = TRUE, disp.bal.tab = TRUE)

    model$Balance$Diff.Adj = model$Balance$Diff.Un
  }

    #print(attributes(model)$print.options$co.names)
  # Create resulting variables
  variablesResult = list()
  for(name in attributes(model)$print.options$co.names)
  {
    variable = list()
    variable["name"] = name[["component"]][1]
    variable["type"] = ifelse(length(name$type) > 1, "factor", "numeric")
    # Store levels or create an artificial one (variable name)
    variable["levels"] = ifelse(length(name$type) > 1, list(levels(dfGroup[[ paste(variable["name"]) ]])), c(variable["name"])) 
    balance = list()
    variable[["balance"]] = list()
    
    variablesResult[[ paste(variable["name"]) ]] = variable
  }
  
  #print(model$print.options$co.names)
  
  indexVariables = 1
  for(variableName in attributes(model)$print.options$co.names)
  {
    variableBaseName = variableName$component[1]
    variable = variablesResult[[variableBaseName]]
    
    levelName = variable$name
    if(variable$type == "factor")
    {
      levelName = variableName$component[3]
    }
    
    # Fill balance values into the variables
    levelBalance = list()
    levelBalance[["M.0.Un"]] = model$Balance$M.0.Un[[indexVariables]]
    levelBalance[["M.1.Un"]] = model$Balance$M.1.Un[[indexVariables]]
    levelBalance[["Diff.Un"]] =  model$Balance$Diff.Un[[indexVariables]]
    levelBalance[["V.Ratio.Un"]] = model$Balance$V.Ratio.Un[[indexVariables]]

    levelBalance[["M.0.Adj"]] = model$Balance$M.0.Adj[[indexVariables]]
    levelBalance[["M.1.Adj"]] = model$Balance$M.1.Adj[[indexVariables]]
    levelBalance[["Diff.Adj"]] =  model$Balance$Diff.Adj[[indexVariables]]
    levelBalance[["V.Ratio.Adj"]] = model$Balance$V.Ratio.Adj[[indexVariables]]
    
    balance = variable[["balance"]]
    balance[[levelName]] = levelBalance
    variable[["balance"]] = balance
    
    variablesResult[[variableBaseName]] = variable
    
    indexVariables = indexVariables + 1
  } # for
  
  #print(variablesResult)
  
  # convert to vector
  variablesBalanced = list()
  for(variable in variablesResult)
  {
    variablesBalanced = c(variablesBalanced, list(variable))
  }
  
  # Compute weighted treated distributions 
  distributionsTreated = balanceModel$distributionsTreated
  queryT = paste0(treat, "=='", balanceModel$treatment$levelTreatment, "'")
  dataframeTreated = subset(dfGroup, eval(parse(text=paste(queryT))))
  
  #print('distributionsTreated:')
  #print(head(distributionsTreated))

  #print('dataframe')
 # print(str(head(dataframeTreated)))

  if(length(covs) == 0)
  {
    distributionsTreated = computeBalanceModelDistributions(distributionsTreated, dataframeTreated, FALSE)
    balanceModel[["distributionsTreated"]] = distributionsTreated
    
    #print(distributionsTreated)
    
    #Compute (unweighted) control distributions
    distributionsControl = balanceModel$distributionsControl
    queryU = paste0(treat, "=='", balanceModel$treatment$levelControl, "'")
    dataframeControl = subset(dfGroup, eval(parse(text=paste(queryU))))
    distributionsControl = computeBalanceModelDistributions(distributionsControl, dataframeControl, FALSE)
    balanceModel[["distributionsControl"]] = distributionsControl
  }

  if(length(covs) > 0)
  {
    distributionsTreated = computeBalanceModelDistributions(distributionsTreated, dataframeTreated, TRUE)
    balanceModel[["distributionsTreated"]] = distributionsTreated
    
    #print(distributionsTreated)
    
    #Compute (unweighted) control distributions
    distributionsControl = balanceModel$distributionsControl
    queryU = paste0(treat, "=='", balanceModel$treatment$levelControl, "'")
    dataframeControl = subset(dfGroup, eval(parse(text=paste(queryU))))
    distributionsControl = computeBalanceModelDistributions(distributionsControl, dataframeControl, TRUE)
    balanceModel[["distributionsControl"]] = distributionsControl
  }

  #print(distributionsControl)

  # #balanceModel[["variables"]] # fix this!
  balanceModel[["variablesBalanced"]] = variablesBalanced
  
  print("computeBalanceModel done")

  return(balanceModel)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeGroupSizes
computeGroupSizes = function(groups)
{
  print("computeGroupSizes")
  
  prepareDataFrames()

  groups = fromJSON(groups)
  groupsUpdated = list()
  
  tryCatch({
      maintable = eval(as.name("main"), .GlobalEnv)
      variablestypestable = eval(as.name("variablestypes"), .GlobalEnv)
      nRowsMainTable = nrow(maintable)
      
      
      for(group in groups)
      {
        dfGroupQuery = group$query
        dfGroup = subset(maintable, eval(parse(text=paste(dfGroupQuery))))  
        nRowsGroup = nrow(dfGroup)
        
        group["size"] = nRowsGroup
        
        groupProperties = list()
          groupProperties["maintable_rowcount"] = nRowsMainTable
        group[["properties"]] = groupProperties
        
        groupsUpdated = c(groupsUpdated, list(group))
      }
    }, 
    error=function(e){
      print(e)
    })
  
  #print(group)
  
  result = list()
  result[["groups"]] = groupsUpdated
  
  print("computeGroupSizes done")

  return(result)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeEffectsModels 
computeEffectsModels = function(effectsModels)
{
  print("computeEffectsModels")
  prepareDataFrames()
  
  effectsModels = fromJSON(effectsModels)
  
  #print( unname(effectsModels))
  
  effectModelsComputed = list()
  for(effectsModel in unname(effectsModels))
  {
    tryCatch({
      computedEffectsModel = computeEffectModel(effectsModel)
      effectModelsComputed = c(effectModelsComputed, list(computedEffectsModel))
    }, 
    error=function(e){
      print(e)
    })
  }
  
  result = list()
  result[["effectsModels"]] = effectModelsComputed
         
  print("computeEffectsModels done")
  
  return(result)
}

computeEffectModel = function(effectsModel)
{
  #print("computeEffectsModel")

  balanceModel = effectsModel$balanceModel
  group = balanceModel$group
  treatment = balanceModel$treatment
  effect = effectsModel$effect

  effectSize = 0
  effectSE = 0
  PValue = 1
  glassDelta = 0

  # Group and balance variables
  treatname = balanceModel$treatment$variable$name
  estmethod = treatment$estimationMethod
  covsBalance = unlist(lapply(balanceModel$variablesBaseline, function(x){return(x$name)}))
  
  # covsInGroupQuery = all.vars(parse(text=paste(group$query)))
  # nocovs = list("p.score", "ID", "computedweight", unlist(covsInGroupQuery))
  prepareDataFrames()

  # Effect variables
  maintable = eval(as.name("main"), .GlobalEnv)
  variablestypestable = eval(as.name("variablestypes"), .GlobalEnv)
  dfGroup = subset(maintable, eval(parse(text=paste(group$query))))

  #Compute propensity scores (methods: logistic regression); need sampling weights!
  options(survey.lonely.psu = "adjust")
  # need sampling weights
  designPs = svydesign(ids=~0, strata=~eval(parse(text=paste(treatname))), data = dfGroup)
  fps =  build.formula(treatname, c(effect$variable$name, covsBalance))
  psModel = svyglm(fps, design=designPs, family=quasibinomial)
  pScores = fitted(psModel)
  dfGroup$pScore = pScores

  print(paste("Estimation method:", estmethod))
  if(estmethod == "ATT")
  {
    dfGroup$computedweight <- ifelse(dfGroup[[treatname]] == balanceModel$treatment$levelTreatment, 
                                      1, dfGroup$pScore/(1-dfGroup$pScore))
  }
 
  if(estmethod == "ATE")
  {
    dfGroup$computedweight <- ifelse(dfGroup[[treatname]] == balanceModel$treatment$levelTreatment, 
                                      1/dfGroup$pScore, 1/(1-dfGroup$pScore))  
  }

  estimationDesign = svydesign(ids=~0, strata=~eval(parse(text=paste(treatname))), data = dfGroup, 
                                  weights=dfGroup[["computedweight"]])

  estformula = build.formula(effect$variable$name, treatname)

  # regres = svyglm(formula=estformula, design=estimationDesign)
  # sumregres = summary(regres)
  # estimate = sumregres$coefficients[[2,  "Estimate"]]
  # eststderror = sumregres$coefficients[[2,  "Std. Error"]]
  # pvalue = sumregres$coefficients[[2,  "Pr(>|t|)"]]

  # estimationVarsDesign = as.svrepdesign(estimationDesign)
  # weightedVars = svyby(formula=~eval(parse(text=paste(effect$variable$name))), by=~eval(parse(text=paste(treatname))), 
  #                       design=estimationVarsDesign, FUN=svyvar)
  # varianceUntreated = weightedVars[paste(balanceModel$treatment$levelControl), 2]

  # effectSize = estimate
  # effectSE = eststderror
  # PValue = pvalue
  # glassDelta = estimate / sqrt(varianceUntreated)


  #print("***")

  # Violin plot data ---------------------------------------
  treatLevels = levels(dfGroup[[paste(treatname)]])
  levelT =  paste(balanceModel$treatment$levelTreatment)
  levelU =  paste(balanceModel$treatment$levelControl)
  levelLabelU = levelU
  levelLabelT = levelT
  
  queryT = paste0(treatname, "=='", levelT, "'")
  queryU = paste0(treatname, "=='", levelU, "'")

  dfT = subset(dfGroup, eval(parse(text=paste(queryT))))
  dfU = subset(dfGroup, eval(parse(text=paste(queryU))))

  # Compute differences
  surveyDesignT = svydesign(id=~1, data=dfT)
  surveyDesignU = svydesign(id=~1, data=dfU)
  
  psFormula = "pScore + I(pScore^2) + I(pScore^3)"
  fT = formula(paste(effect$variable$name, "~", psFormula))
  fU = formula(paste(effect$variable$name, "~", psFormula))

  # For binomial and Poisson families use family=quasibinomial() and family=quasipoisson() 

  modelT = svyglm(fT, design = surveyDesignT)
  modelU = svyglm(fU, design = surveyDesignU)
  
  print(paste("method", estmethod))
  if(estmethod == "ATT")
  {

    # ATT  
    dfPScoresNewt1 = data.frame(pScore=with(dfGroup, pScores[eval(parse(text=paste(queryT)))]))
    Yt1 = predict(modelT,
                  newdata=dfPScoresNewt1,
                  vcov = TRUE, type="response")
    fPScoresNewt0 = data.frame(pScore=with(dfGroup, pScores[eval(parse(text=paste(queryT)))]))
    Yt0 = predict(modelU,
                  newdata=fPScoresNewt0,
                  vcov = TRUE, type="response")
  }
  if(estmethod == "ATE")
  {

    # ATE  
    dfPScoresNewt1 = data.frame(pScore=with(dfGroup, pScores[eval(parse(text=paste(queryT)))]))
    Yt1 = predict(modelT,
                  newdata=dfPScoresNewt1,
                  vcov = TRUE, type="response")
    fPScoresNewt0 = data.frame(pScore=with(dfGroup, pScores[eval(parse(text=paste(queryU)))]))
    Yt0 = predict(modelU,
                  newdata=fPScoresNewt0,
                  vcov = TRUE, type="response")
  }

  ttest = t.test(Yt0, Yt1)

  meanU = ttest$estimate[["mean of x"]]
  meanT = ttest$estimate[["mean of y"]]
  varT = var(Yt1)
  varU = var(Yt0)

  PValue = ttest$p.value
  glassDelta = (meanT - meanU) / varU
  effectSize = (meanT - meanU)
  effectSE = ttest$stderr

  dfYt0 = data.frame(Yt0)
  dfYt1 = data.frame(Yt1)

  dfGroup1 = data.frame(response = dfYt0[, 1])
  dfGroup1['group'] = 1
  dfGroup2 = data.frame(response = dfYt1[, 1])
  dfGroup2['group'] = 2
  combinedDf <- rbind(dfGroup1, dfGroup2)

  plotViolinU = ggplot(combinedDf, aes(x = "group", y = response)) + 
    geom_violin(trim=FALSE, fill="white", scale="area") +
    geom_boxplot(width=0.1)+
    stat_summary(fun.y=mean) +
    theme_classic()
  
  plotDataU = ggplot_build(plotViolinU)
  violDataU = plotDataU$data[1]
  boxPlotDataU = plotDataU$data[2]
  
  violinDataExtractedU = list()
  violinDataExtractedU["n"] = 1#violDataU[[1]]$n
  violinDataExtractedU[["y"]] = violDataU[[1]]$y
  violinDataExtractedU["levelLabel"] = levelLabelU
  violinDataExtractedU[["violinwidth"]] = violDataU[[1]]$violinwidth
  violinDataExtractedU[["boxPlot"]] = boxPlotDataU

  # Violin plot data ---------------------------------------
  plotViolinT = ggplot(data.frame(Yt1), aes(x = "t", y = response)) + 
    geom_violin(trim=FALSE, fill="white", scale="area") +
    geom_boxplot(width=0.1)+
    stat_summary(fun.y=mean) +
    theme_classic()
  
  plotDataT = ggplot_build(plotViolinT)
  violDataT = plotDataT$data[1]
  boxPlotDataT = plotDataT$data[2]

  print(boxPlotDataT)

  violinDataExtractedT = list()
  violinDataExtractedT["n"] = 1#violDataT[[1]]$n
  violinDataExtractedT[["y"]] = violDataT[[1]]$y
  violinDataExtractedT["levelLabel"] = levelLabelT
  violinDataExtractedT[["violinwidth"]] = violDataT[[1]]$violinwidth
  violinDataExtractedT[["boxPlot"]] = boxPlotDataT
  
  distributionEffectU = list()
  distributionEffectU[["group"]] = effectsModel$balanceModel$group
  distributionEffectU[["variable"]] = effect
  distributionEffectU[["violin"]] = violinDataExtractedU
  
  distributionEffectT = list()
  distributionEffectT[["group"]] = effectsModel$balanceModel$group
  distributionEffectT[["variable"]] = effect
  distributionEffectT[["violin"]] = violinDataExtractedT
  
  #effectsModelResult = list()
  #effectsModel[["balanceModel"]] = balanceModel
  #effectsModel[["variableEffect"]] = variableEffect
  effectsModel[["distributionU"]] = distributionEffectU
  effectsModel[["distributionT"]] = distributionEffectT

  properties = list()
  properties["effectSize"] = effectSize
  properties["SE"] = effectSE;
  properties["P"] = PValue;
  properties["D"] = glassDelta # glass delta
  effectsModel[["properties"]] = properties

  return(effectsModel)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeCausalModels 
computeCausalModels = function(causalModels, alphaValue)
{
  print("computeCausalModels")
  
  #set.seed(489)
  
  causalModels = fromJSON(causalModels)
  alphaValue = fromJSON(alphaValue)
  #alphaValue = 0.01
  
  causalModelsComputed = list()
  for(causalModel in unname(causalModels))
  {
    tryCatch({
      computedCausalModel = computeCausalModel(causalModel, alphaValue)
      causalModelsComputed = c(causalModelsComputed, list(computedCausalModel))
    }, 
    error=function(e){
      print(e)
    })
  }
  
  result = list()
  result[["causalModels"]] = causalModelsComputed
  
  #print(result)
  print("computeCausalModels done")
  
  return(result)
}

computeCausalModel = function(causalModel, alphaValue)
{
  print("computeCausalModel")

  print("group, alpha:")
  #print(causalModel$group)
  alphaValue = as.numeric(alphaValue)
  
  #variable = distribution$variable
  group = causalModel$group
  dfGroupQuery = group$query
  treat = causalModel$treatment$variable$name

  maintable <- eval(as.name("main"), .GlobalEnv)
  variablestypestable = eval(as.name("variablestypes"), .GlobalEnv)
  
  dfGroup = subset(maintable, eval(parse(text=paste(dfGroupQuery))))
  covsInGroupQuery = all.vars(parse(text=paste(group$query)))

  # For the demo speed up the algorithm by sampling
  dfGroup = sample_n(dfGroup, 500)  

  # convert character to factors to make sure
  dfGroup[sapply(dfGroup, is.character)] <- lapply(dfGroup[sapply(dfGroup, is.character)], as.factor)

  # We regard up to 5 values a factor
   col_names <- sapply(dfGroup, function(col) length(unique(col)) < 5)
   dfGroup[ , col_names] <- lapply(dfGroup[ , col_names] , factor)

  # Remove 1 level factors
  dfGroup = dfGroup[, sapply(dfGroup, nlevels) > 1]
  

  #Drop some columns we don't need
  drops = list("p.score", "ID", "computedweight", unlist(covsInGroupQuery))
  dfGroupFiltered = dfGroup[ , !(names(dfGroup) %in% drops)]
  
  gSkeleton = MXM::pc.skel(dfGroupFiltered, method = "comb.mm", alpha = alphaValue, R=2) ## skeleton
  gSkelDirected = MXM::pc.or(gSkeleton)
  gDirected <- gSkelDirected$G
  
  # Directed edges
  adjList = as.matrix(apply(gDirected==2, 1 ,function(a) (colnames(gDirected)[a])))

  adjListResult = list()
  for(i in 1:length(adjList))
  {
    varName = names(adjList[i, ])[1]
    adjs = unname(adjList[varName, ])
    adjs = strsplit(unlist(adjs), " ")

    if(length(adjs) > 0)
    {
      adjListResult[[varName]] = adjs  
    }
  }
  print("****")
  
  causalModel[["minedEdges"]] = adjListResult
  
  #print(causalModel)
  
  return(causalModel)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeAdjustmentModels 
computeAdjustmentModels = function(causalModels)
{
  print("computeAdjustmentModels")
  
  #set.seed(489)
  
  causalModels = fromJSON(causalModels)

  print("count causalmodels")
  print(length(causalModels))
  
  adjustmentModelsComputed = list()
  for(causalModel in unname(causalModels))
  {
    tryCatch({
      computedAdjustmentModel = computeAdjustmentModelsAux(causalModel)
      adjustmentModelsComputed = c(adjustmentModelsComputed, list(computedAdjustmentModel))
    }, 
    error=function(e){
      print(e)
    })
  }
  
  result = list()
  result[["adjustmentModels"]] = adjustmentModelsComputed
  
  #print(result)
  print("computeAdjustmentModels done")
  
  return(result)
}

computeAdjustmentModelsAux = function(causalModel)
{
  print("computeCausalModelAdjustmentSets")

  adjustmentSetsResult = list();
 
  arrows = ""
  graphUser = causalModel$graphUser
  
  for(edge in graphUser$edges)
  {
    firstNode = edge$firstNode
    secondNode = edge$secondNode

    nameSourceNode = firstNode$label
    nameTargetnode = secondNode$label

    if( length(nameSourceNode) != 0 && length(nameTargetnode) != 0 )
    {
      arrow = paste(nameSourceNode, " -> ", nameTargetnode, ";", collapse = "")
      arrows = paste(arrows, arrow, collapse="")
    }
  }
  
  dag = paste("dag {", arrows, "}", collapse = "")
  g <- dagitty(dag)

#  print(g)
  # dag = "dag{ romantic -> g3 ; age -> romantic ; age -> g3 ; absences -> romantic; absences -> g3}"
  # g <- dagitty(dag)
  # adjustmentSets = adjustmentSets( g, "romantic", "g3" )

  print("exposure")
  print(causalModel$treatment$variable$name)
  print("outcome")
  print(causalModel$effect$variable$name)

  setsAdjustments = adjustmentSets( g, 
                                   paste(causalModel$treatment$variable$name), 
                                   paste(causalModel$effect$variable$name),
                                   type="minimal")

  #setsAdjustments = list()
  print("#adjustment sets:")
  print(length(setsAdjustments))
  print(setsAdjustments[1])
  print(str(setsAdjustments))

  #setsAdjustments[2] = setsAdjustments[1]

  print(setsAdjustments)

  # Convert to vector
  vecAdjustmentSets = list()
  adjustmentSetNew = list()
  for(adjustmentSetCurrent in setsAdjustments)
  {
    print(adjustmentSetCurrent)
    print(str(adjustmentSetCurrent))
    variablesResult = list()
    for(variablename in adjustmentSetCurrent)
    {
      variablesResult = c(variablesResult, list(variablename))
    }
    
    print('variablesResult')
    print(variablesResult)
    adjustmentSetNew[["nodeNames"]] = variablesResult
    adjustmentSetNew[["group"]] = causalModel$group
    adjustmentSetNew[["treatment"]] = causalModel$treatment
    adjustmentSetNew[["effect"]] = causalModel$effect

    print("adjustmentSetNew$nodenames")
    print(adjustmentSetNew$nodeNames)
    
    vecAdjustmentSets = c(vecAdjustmentSets, list(adjustmentSetNew))
  }

  paths = paths(g, 
                  causalModel$treatment$variable$name, 
                  causalModel$effect$variable$name,
                  limit = 999999)
  openPaths = paths$open
 
  print('#paths:')
  print(length(paths))
  #print(str(paths))

  print('#open paths:')
  print(length(openPaths))
  #print(str(openPaths))
  
  result = list()
  result[["causalModel"]] = causalModel
  result[["adjustmentSets"]] = vecAdjustmentSets
  #result[["adjustmentSets"]] = adjustmentSets
  result[["pathsInfo"]] = paths
 
  #print(result)

  print("computeCausalModelAdjustmentSets done")

  return(result)
}

###############################################################################
#* @serializer unboxedJSON
#* @post /computeMarkovModels
computeMarkovModels = function(causalModels, allVariables)
{
  print("computeMarkovModels")
  
  #set.seed(489)
  
  causalModels = fromJSON(causalModels)
  allVariables = fromJSON(allVariables)
  
  markovModelsComputed = list()
  for(causalModel in unname(causalModels))
  {
    tryCatch({
      computedMarkovModel = computeMarkovModelsAux(causalModel, allVariables)
      markovModelsComputed = c(markovModelsComputed, list(computedMarkovModel))
    }, 
    error=function(e){
      print(e)
    })
  }
  
  result = list()
  result[["markovModels"]] = markovModelsComputed
  
  #print(result)
  print("computeMarkovModels done")
  
  return(result)
}

markov_blanket_fix <- function(graph, nodes) {
  map_nodes <- function(nodes, .f) {
    nodes %>% 
      map(.f, x = graph) %>% 
      unlist()
  }
  nodes %>% 
    map_nodes(children) %>% 
    map_nodes(parents) %>% 
    union(nodes %>% map_nodes(parents)) %>% 
    union(nodes %>% map_nodes(children)) %>% 
    setdiff(nodes) %>% 
    sort()
}

computeMarkovModelsAux = function(causalModel, allVariables)
{
  print("computeMarkovModelsAux")

  #print(causalModel)
  #print(allVariables)

  markovBlankets = list();

  graphUser = causalModel$graphUser

  arrows = ""
  
  for(edge in graphUser$edges)
  {
    firstNode = edge$firstNode
    secondNode = edge$secondNode

    nameSourceNode = firstNode$label
    nameTargetnode = secondNode$label

    if( length(nameSourceNode) != 0 && length(nameTargetnode) != 0 )
    {
      arrow = paste(nameSourceNode, " -> ", nameTargetnode, ";", collapse = "")
      arrows = paste(arrows, arrow, collapse="")
    }
  }
  
  dag = paste("dag {", arrows, "}", collapse = "")
  g <- dagitty(dag)

  treatmentName = paste(causalModel$treatment$variable$name)
  effectName = paste(causalModel$effect$variable$name)

  #mbresult = markovBlanket(g, treatmentName)
  mbresult = markov_blanket_fix(g, treatmentName)

  variabesMB = list()
  for(varname in mbresult)
  {
    variable = allVariables[varname]
    variabesMB[[length(variabesMB) + 1]] = variable
  }

  print("******")
  print("computeMarkovModelsAux done")

  result = list()
  result[["causalModel"]] = causalModel
  result[["markovBlanket"]] = variabesMB

  return(result)
}

