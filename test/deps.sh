#!/bin/bash

# Download
echo "Downloading Boost..."
wget https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.gz

# Extract the compressed file
echo "Extracting the compressed file..."
tar -zxvf boost_1_83_0.tar.gz

# Clean up
echo "Cleaning up..."
rm boost_1_83_0.tar.gz

echo "Boost has been successfully extracted!"
# Copy
cp -r boost_1_83_0/boost /usr/local/include/boost
cp -r boost_1_83_0/libs /usr/local/include/libs
rm -rf boost_1_83_0