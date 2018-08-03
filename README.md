# irods-microservices

iRODS microservices for the RIT project

## Dependencies

irods-dev, libssl-dev libcurl4-openssl-dev

## Building

```
cd /microservices
cmake -H. -Bbuild
cmake --build build -- -j3
```

## Packaging TODO: Needs to be updated for CMake procedure

Don't forget to:
* add a new microservice to `packaging/rit-irods-microservices.list.template`
* update the PLUGINVERSION in `VERSION` file

Then execute

```
./packaging/build.sh
```

## Releasing

Tag a release, and upload the binary .deb and .rpm to GitHub.
 
## Acknowledgements
* Special thanks to the [Donders Institute](https://github.com/Donders-Institute) for sharing of `msi_json_arrayops`, `msi_json_objops` and `msi_time_ts2str`
* Special thanks to [irods-contrib](https://github.com/irods/contrib/tree/master/microservices/landing_zone_microservices/msiput_dataobj_or_coll) for creating `msiput_dataobj_or_coll` which formed the base for our modified version.
* Special thanks to [Utrecht University](https://github.com/UtrechtUniversity/irods-uu-microservices/compare/feature/irods-4.2.3) for inspiration on CMake and ported iRODS 4.2.3 microservices