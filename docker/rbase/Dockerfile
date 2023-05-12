FROM ubuntu:latest

ENV DEBIAN_FRONTEND noninteractive
ENV DEBCONF_NONINTERACTIVE_SEEN true

# User
RUN useradd docker \
	&& mkdir /home/docker \
	&& chown docker:docker /home/docker \
	&& addgroup docker staff

# Upgrade
RUN apt update -y && DEBIAN_FRONTEND=noninteractive apt upgrade -y --no-install-recommends

# apt-utils
RUN apt install apt-utils -y

# Install default shizzle
RUN apt-get install -y --no-install-recommends \
	ca-certificates \
	ed \
	fonts-texgyre \
	less \
	locales \
	vim-tiny \
	wget \
&& rm -rf /var/lib/apt/lists/*

## Configure default locale, see https://github.com/rocker-org/rocker/issues/19
RUN truncate -s0 /tmp/preseed.cfg; \
    echo "tzdata tzdata/Areas select Europe" >> /tmp/preseed.cfg; \
    echo "tzdata tzdata/Zones/Europe select Amsterdam" >> /tmp/preseed.cfg; \
    debconf-set-selections /tmp/preseed.cfg
	
RUN echo "en_US.UTF-8 UTF-8" >> /etc/locale.gen \
	&& locale-gen en_US.utf8 \
	&& /usr/sbin/update-locale LANG=en_US.UTF-8

ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8

## Use Debian unstable via pinning -- new style via APT::Default-Release
#RUN echo "deb http://http.debian.net/debian sid main" > /etc/apt/sources.list.d/debian-unstable.list \
        #&& echo 'APT::Default-Release "testing";' > /etc/apt/apt.conf.d/default \
 #       && echo 'APT::Install-Recommends "false";' > /etc/apt/apt.conf.d/90local-no-recommends


# Install DEPS
# ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime
# RUN apt update -y && DEBIAN_FRONTEND=noninteractive  apt install -y --no-install-recommends \
# 				build-essential \
#                 libopenblas-openmp-dev \
# 				littler	\
# 				gnupg2 \
# 				software-properties-common

RUN apt update -y && apt install -y --no-install-recommends software-properties-common dirmngr
RUN wget -qO- https://cloud.r-project.org/bin/linux/ubuntu/marutter_pubkey.asc | tee -a /etc/apt/trusted.gpg.d/cran_ubuntu_key.asc
RUN add-apt-repository "deb https://cloud.r-project.org/bin/linux/ubuntu $(lsb_release -cs)-cran40/"
# Install R
#RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys E298A3A825C0D65DFD57CBB651716619E084DAB9
#RUN add-apt-repository 'deb https://cloud.r-project.org/bin/linux/ubuntu focal-cran40/'
RUN apt update -y && apt install -y r-cran-littler \
		r-base \
		r-base-core \
		r-base-dev \
		r-recommended  \
		&& rm -rf /tmp/downloaded_packages/ /tmp/*.rds \
		&& rm -rf /var/lib/apt/lists/*

RUN R -e "install.packages('docopt')"

#CMD ["R"]

# Install plumber
RUN apt-get update -qq && apt-get install -y \
  git-core \
  libssl-dev \
  libcurl4-gnutls-dev

RUN apt-get update -qq && apt-get install -y libsodium-dev

RUN R -e "install.packages('plumber')"

EXPOSE 8000
ENTRYPOINT ["R", "-e", "pr <- plumber::plumb(commandArgs()[4]); pr$run(host='0.0.0.0', port=8000)"]
#CMD ["/usr/local/lib/R/site-library/plumber/examples/04-mean-sum/plumber.R"]