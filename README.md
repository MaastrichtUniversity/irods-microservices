# irods-microservices

iRODS microservices for the RIT project

## Dependencies

irods-dev, libssl-dev libcurl4-openssl-dev

## Building

```
make
```

## Packaging

Don't forget to add a new microservice to `packaging/rit-irods-microservices.list.template`

```
./packaging/build.sh
```

## Releasing

Tag a release, and upload the binary .dev to GitHub. 