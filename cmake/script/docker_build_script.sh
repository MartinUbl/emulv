#!/bin/sh

log_dir="$(dirname "$0")"

# Directory with the project we want to compile - the directory that contains the root CMakeLists.txt file
src=$(builtin cd "$PWD/../.."; pwd)

###########################################################################
# PROMPTING USER TO CONFIRM THE SRC PATH
###########################################################################

# Prompt the user for confirmation of the default path
read -p "The project directory is set to: $src. Do you want to use this path? (y/n): " answer
case $answer in
    [Yy]* ) ;;
    [Nn]* )
        # User wants to enter a custom path
        while true; do
            read -p "Enter the custom source directory path or 'exit' to quit: " custom_src
            
            if [ "$custom_src" == "exit" ]; then
                exit
                elif [ -d "$custom_src" ]; then
                src="$(realpath "$custom_src")"  # Get the absolute path of the custom source directory
                break
            else
                echo "Invalid directory path. Please try again or type 'exit' to quit."
            fi
        done
    ;;
    * )
        echo "Invalid input. Please answer 'y' for yes or 'n' for no."
        exit 1
    ;;
esac

###########################################################################
# DEFINE PLATFORMS ARRAY
###########################################################################

# List of all available platforms (docker tags) for which we want to compile
PLATFORMS=("6.5-gcc-aqt" "6.5-macos-aqt" "6.5-mingw-aqt")

# Display all platform names
echo "Available platforms:"
for i in "${!PLATFORMS[@]}"; do
    echo "$((i+1)). ${PLATFORMS[$i]}"
done

###########################################################################
# CONFIRMING WHICH PLATFORMS USER WANTS TO COMPILE FOR
###########################################################################

# Prompt the user to choose whether to compile for all platforms
read -p "Do you want to compile for all platforms? (y/n): " compile_all

# Initialize the IMAGES array based on user input
IMAGES=()

if [ "$compile_all" == "y" ] || [ "$compile_all" == "Y" ]; then
    IMAGES=("${PLATFORMS[@]}")
else
    # Generate platform options dynamically
    for i in "${!PLATFORMS[@]}"; do
        echo "$((i+1)). ${PLATFORMS[$i]}"
    done

    # Prompt the user to choose which platforms they want to compile for
    read -p "Enter the numbers of the platforms you want (e.g., '1,2', '1,3'): " selected_platforms

    # Initialize IMAGES based on user selection
    IFS=',' read -ra platform_indices <<< "$selected_platforms"
    for index in "${platform_indices[@]}"; do
        if (( index >= 1 && index <= ${#PLATFORMS[@]} )); then
            IMAGES+=("${PLATFORMS[$((index-1))]}")
        fi
    done
fi

# Display selected platforms
echo "Selected platforms:"
for platform in "${IMAGES[@]}"; do
    echo "$platform"
done

###########################################################################
# RUNNING DOCKER IMAGES
###########################################################################

for image in "${IMAGES[@]}"; do
    project="$(basename "$src")"
    
    cmd=
    
    # Install deps for non-console apps on gcc
    if [ "$(echo "$image" | grep 'gcc-')" -a -z "$(echo "$project" | grep 'console')" ]; then
        cmd='sudo apt update ; sudo apt install -y libgl-dev;'
    fi
    cmd="$cmd qt-cmake ./project -G Ninja -B ./build; cmake --build ./build;"
    
    archs=
    if echo "$image" | grep -q 'gcc-'; then
        cmd="$cmd "'linuxdeploy --plugin qt -e "$(find ./build -maxdepth 1 -type f -executable)" --appdir ./build/deploy;'
    fi
    if echo "$image" | grep -q 'mingw-'; then
        cmd="$cmd windeployqt --qmldir ./project --dir build/deploy --libdir build/deploy/libs --plugindir build/deploy/plugins build/*.exe;"
    fi
    if echo "$image" | grep -q 'macos-'; then
        # Deploy only GUI apps since there will be no .app bundle for console project
        if [ "x$(echo "$project" | grep 'console')" = 'x' ]; then
            cmd="$cmd macdeployqt ./build/*.app -verbose=1 -dmg -qmldir=./project;"
        fi
        archs="aarch64"
    fi
    cmd="$cmd ls -alh build;"
    
    for arch in '' $archs; do
        logfile="$log_dir/docker_build_logs/${image}_${project}_$arch.log"

        #Git needs to be installed to pull project dependencies
        install_git_cmd='sudo apt update ; sudo apt install -y git;'

        #Add copying build files to the commnad
        finalcmd="$install_git_cmd $cmd cp -a ./build/. /home/user/project/docker_build_${image}${arch}_${project};"
        
        mkdir -p $(dirname "$logfile")
        
        echo "--------------------------------------------------------------"
        echo -n "Running $image $project $arch..."
        # Debug print:
        #printf "\n\n"
        #printf "Running with this docker command:\n"
        #echo "docker run -it --rm -v" "$src:/home/user/project:" "stateoftheartio/qt6:$image" "sh -xec" "BUILD_ARCH=$arch $finalcmd"
        #printf "\n\n"
        
        
        docker run -it --rm -v "$src:/home/user/project" stateoftheartio/qt6:$image \
        sh -xec "BUILD_ARCH=$arch $finalcmd" > "$logfile" 2>&1
        if [ "$?" -eq 0 ]; then
            echo "SUCCESS"
        else
            echo "FAIL - $logfile"
            errors=$(($errors+1))
        fi
        
    done
    
done

exit $errors
