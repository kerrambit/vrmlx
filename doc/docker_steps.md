# Docker

[Docker](https://www.docker.com/) is a tool that is used to automate the deployment of applications in lightweight containers so that applications can work efficiently in different environments in isolation.

## Steps

- Make sure your systems has *Docker* installed.
- Download our [Dockerfile](../Dockerfile) - copy/paste should be enough.
- Build the image using ```docker build -t <IMAGE_NAME> .``` .
- You can use following build arguments:
    - ```--build-arg BRANCH=<BRANCH_NAME>``` - specify the branch in this repository, image will be build from this branch.
    - ```--build-arg BUILD_CONFIGURATION=<NAME>``` - specify the type of [build configuration](../README.md#build).
    - ``` --build-arg CACHEBUST=$(date +%s)``` - does not cache the *vrmlxpy* clone from repository and updates remote changes.
- After the image is built, you should be able to create and run container.
<!-- Empty line here -->
- Before running the container, make sure you have created two folders ```input``` and ```output``` on your current path. It is because we have to bind folders where the converted data and data to convert will be searched for.
- Insert into ```input``` folder your ```.wrl``` files and the configuration file (it can look like [this](../vrmlxpyConfig.json.example)).
- There are two applications you can use - single file conversion and bulk conversion, respectively:
    - ```docker run -v $(pwd)/input:/app/input -v $(pwd)/output:/app/output <IMAGE_NAME> /app/vrmlxpyConversionApp /app/input/<YOUR_VRML_FILE> /app/output/<OUTPUT_FILENAME> /app/input/<CONFIG_FILE>```.
    <!-- Empty line here -->
    - ```docker run -v $(pwd)/input:/app/input -v $(pwd)/output:/app/output <IMAGE_NAME> /app/vrmlxpyBulkConversionApp /app/input/ /app/output/```.
        - **Tip**: the configuration file for bulk conversion has to be inside ```input``` folder and expected name is ```vrmlxpyConfig.json```!
- There is also a [Python script](../scripts/run_vrmlxpy_from_docker.py) which you can run inside the container to make a conversion. The usage is basically the same as for C++ *vrmlxpyConversionApp* program.

## Other commands

- Another way how to work with a container is to switch to an interactive mode and do conversions inside the container: ```docker run -it -v $(pwd)/input:/app/input -v $(pwd)/output:/app/output <IMAGE_NAME> /bin/bash```

- Do not forget to remove unused containers! One option is to run containers with option ```--rm```. Container is removed right after it finished (it can take some time so be prepared that converion itself may not take that much time but container creating/removing does). You can also diplay all containers from image like this ```docker ps -a --filter "ancestor=<IMAGE_NAME>"``` and one container is removed with ```docker rm <CONTAINER_ID>``` (or for running container ```docker stop <CONTAINER_ID> && docker rm <CONTAINER_ID>```).