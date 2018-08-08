# irods-microservices

iRODS microservices for the RIT project

## Dependencies

##### Ubuntu & Debian
```
# Install via packet manager
apt-get install \
libssl-dev \
libcurl4-openssl-dev \
g++ \
make \
irods-dev \
irods-externals-clang-runtime3.8-0 \
irods-externals-clang3.8-0

# Then install a recent version of CMake
wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.sh && \
sudo sh cmake-3.12.0-Linux-x86_64.sh --prefix=/usr/local --exclude-subdir
```

##### CentOS & RedHat
```
# Install via packet manager
yum install \
openssl-devel \
libcurl-devel \
gcc-c++ \
make \
rpm-build \
irods-devel \
irods-externals-clang-runtime3.8-0 \
irods-externals-clang3.8-0
    
# Then install a recent version of CMake
wget https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.sh && \
sudo sh cmake-3.12.0-Linux-x86_64.sh --prefix=/usr/local --exclude-subdir
```

## Building

```
cd /microservices/
mkdir build && cd build
cmake ..              # Generates all the make files
make                  # Compiles the project based on all the make files
make install          # Installs the .so files into the microservices plugin directory
```

## Packaging

```
cd /microservices/build
make package
```

Find your .rpm or .deb package in the `/microservices/build` directory. <br><br>
The name of the package is constructed like this: `rit-irods-microservices-IRODS VERSION_MSI RELEASE VERSION-1.rpm|deb`

## Releasing

Tag a release, and upload the binary .deb and .rpm to GitHub.
 
## Acknowledgements
* Special thanks to the [Donders Institute](https://github.com/Donders-Institute) for sharing of `msi_json_arrayops`, `msi_json_objops` and `msi_time_ts2str`
* Special thanks to [irods-contrib](https://github.com/irods/contrib/tree/master/microservices/landing_zone_microservices/msiput_dataobj_or_coll) for creating `msiput_dataobj_or_coll` which formed the base for our modified version.
* Special thanks to [Utrecht University](https://github.com/UtrechtUniversity/irods-uu-microservices/tree/feature/irods-4.2.3) for inspiration on CMake and ported iRODS 4.2.3 microservices