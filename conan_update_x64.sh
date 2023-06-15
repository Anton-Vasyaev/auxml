#!/bin/bash

current_directory=$(pwd)
conan_dir="${current_directory}/.main_cache/conan"
echo ${conan_dir}

conan install conanfile.txt -if ${conan_dir}/x64/Debug   -s build_type=Debug   --build=missing
conan install conanfile.txt -if ${conan_dir}/x64/Release -s build_type=Release --build=missing