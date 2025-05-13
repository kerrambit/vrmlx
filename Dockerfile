# Use Ubuntu as base image.
FROM ubuntu:24.04 AS builder

# Install prerequisities.
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-all-dev \
    libcgal-dev \
    git \
    ninja-build \
    && rm -rf /var/lib/apt/lists/*

# Set build configuration argument.
ARG BUILD_CONFIGURATION=Production

# Copy the entire project into the container.
COPY . /app
WORKDIR /app

# Build the project.
ARG BUILD_CONFIGURATION=Production
RUN cmake --preset ${BUILD_CONFIGURATION}
RUN cmake --build out/build/${BUILD_CONFIGURATION}

# --------------------------------------- #

# Runtime.
FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y \
    libboost-iostreams1.83.0 \
    libboost-log1.83.0 \
    libboost-thread1.83.0 \
    libboost-filesystem1.83.0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

ARG BUILD_CONFIGURATION=Production

COPY --from=builder /app/out/build/${BUILD_CONFIGURATION}/libtogeom.so /app/
COPY --from=builder /app/out/build/${BUILD_CONFIGURATION}/libvrmlproc.so /app/
COPY --from=builder /app/out/build/${BUILD_CONFIGURATION}/vrmlxpyConversionApp /app/

ENV LD_LIBRARY_PATH=/app:$LD_LIBRARY_PATH

CMD ["/app/vrmlxpyConversionApp", "arg1", "arg2", "arg3"]