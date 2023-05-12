FROM myrbase

RUN apt update && apt install -y cmake
RUN R -e "install.packages('nloptr')"
RUN R -e "install.packages('lme4')"

RUN apt update && apt install -y libgsl-dev
RUN R -e "install.packages('RcppGSL')"
RUN R -e "install.packages('RcppZiggurat')"

RUN R -e "install.packages('R.methodsS3')"
RUN R -e "install.packages('R.oo')"
RUN R -e "install.packages('R.utils')"
RUN R -e "install.packages('R.cache')"

RUN R -e "install.packages('Rfast')"
RUN R -e "install.packages('Rfast2')"
RUN R -e "install.packages('R.rsp')"
RUN apt update && apt install -y libuuid1 uuid-dev
RUN R -e "install.packages('bigmemory')"
RUN R -e "install.packages('MXM')"

RUN R -e "install.packages('jsonlite')"
RUN apt install -y libpq-dev --fix-missing
RUN R -e "install.packages('RPostgreSQL')"
RUN R -e "install.packages('useful')"
RUN R -e "install.packages('cobalt')"
RUN R -e "install.packages('e1071')"
RUN R -e "install.packages('survey')"
RUN R -e "install.packages('dplyr')"
RUN R -e "install.packages('plotrix')"
RUN R -e "install.packages('weights')"
RUN apt update && apt install -y libv8-dev
RUN R -e "install.packages('dagitty')"

CMD ["/api/main.R"]

