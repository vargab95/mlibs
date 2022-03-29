#!/bin/bash

#!/bin/bash

BUILD_DIRECTORY_NAME=build
MODULES=(m_common m_hash m_mem m_str m_list m_map m_args)

function print_help {
    echo "$0 <module|all|help> [--install]"
    echo
    echo "Supported modules are"
    for module in ${MODULES[@]}
    do
        echo "    - $module"
    done
}

function build {
    BUILD_DIR=$1/$BUILD_DIRECTORY_NAME

    if [[ ! -d $BUILD_DIR ]]; then
        mkdir $BUILD_DIR
    fi

    cd $BUILD_DIR
    cmake ..
    make

    if [[ "$INSTALL" == "--install" ]]; then
        sudo make install
    fi

    cd ../..
}

if [ "$#" -lt 1 ]; then
    print_help
    exit 1
elif [ "$#" -eq 2 ]; then
    if [[ "$2" != "--install" ]]; then
        print_help
        exit 2
    fi
elif [ "$#" -gt 2 ]; then
    print_help
    exit 3
fi

INSTALL=$2

case $1 in
    "all")
        for module in "${MODULES[@]}"
        do
            build $module
        done
        ;;
    "help")
        print_help
        ;;
    *)
        module=$1

        declare -A module_map_helper
        for key in "${!MODULES[@]}"
        do
            module_map_helper[${MODULES[$key]}]="$key"
        done

        if [[ -n "${module_map_helper[$module]}" ]]
        then
            build $module
        else
            echo "Not supported module"
        fi
        ;;
esac
