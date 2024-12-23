# Use Rocky Linux as the base image
FROM rockylinux/rockylinux:8 AS build

# Add a label for the image author
LABEL maintainer="arbaudie.it@gmail.com"

# Install necessary tools and libraries
RUN dnf update -y && yum install -y epel-release
RUN dnf install -y git gcc gcc-c++ cmake curl net-tools cjson-devel
RUN dnf --enablerepo=devel install -y libmicrohttpd-devel
#housekeeping time
RUN yum clean all


# Add MariaDB 11.4 repository and install it + devel headers + housekeeping
RUN curl -sSL https://downloads.mariadb.com/MariaDB/mariadb_repo_setup | bash -s -- --mariadb-server-version="mariadb-11.4" && \
    yum install -y MariaDB-server MariaDB-client MariaDB-devel && yum clean all

# Set the working directory
WORKDIR /opt

# Add :opt to the $PATH for easy calling from inside the container
ENV PATH="${PATH}:/opt"

RUN printf "[mariadb]\nplugin-load-add=libjson2sql\nplugin_maturity=unknown\n" > /etc/load_jsonplugin.cnf
RUN printf "#!/bin/bash \n cd /app \n git clone --depth 1 https://github.com/MariaDBJones/json2sql-plugin.git . \n mkdir build \n cd build \n cmake .. \n" > clone
RUN printf "#!/bin/bash \n cd /app/build \n cmake --build . --verbose \n" > build
RUN printf "#!/bin/bash \n cp -f /app/plugin/libjson2sql.so /usr/lib64/mysql/plugin/ \n mariadbd-safe --defaults-extra-file=/etc/load_jsonplugin.cnf & && grep plugin /var/lib/mysql/*.err" > deploy
RUN printf "#!/bin/bash \n mariadb -e 'shutdown;' \n rm -f /usr/lib64/mysql/plugin/libjson2sql.so \n rm -f /app/plugin/libjson2sql.so \n" > clean
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X GET http://127.0.0.1:3000/v1/$1 " > GET
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X DELETE http://127.0.0.1:3000/v1/tables/schema/table/column/value " > DELETE
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X PATCH http://127.0.0.1:3000/v1/tables/schema/table/column/value " > PATCH
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X POST http://127.0.0.1:3000/v1/tables/schema/table " > POST
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X PUT http://127.0.0.1:3000/v1/tables/schema/proc " > PUT
RUN printf "#!/bin/bash \n curl -w '%{http_code}' -X $1 http://127.0.0.1:3000/v1/pouet/pouet/pouet " > FAIL
WORKDIR /app

# Expose the default json2sql port
EXPOSE 3000

# Start command for the container (MariaDB will not auto-start)
CMD ["/bin/bash"]

RUN printf "#!/bin/bash \n cd /app \n git clone --depth 1 https://github.com/SylvainA77/json2sql-plugin.git . \n mkdir build \n cd build \n cmake .. \n" > clone
RUN printf "#!/bin/bash \n rm /usr/lib64/mysql/plugin/libjson2sql.so \n cp -f /app/plugin/libjson2sql.so /usr/lib64/mysql/plugin/ \n mariadbd-safe --defaults-extra-file=/etc/load_jsonplugin.cnf &" > deploy
# dont forget to chmod the scripts !! 

WORKDIR /app

# Expose the default json2sql port
EXPOSE 3000

# Start command for the container (MariaDB will not auto-start)
CMD ["/bin/bash"]
