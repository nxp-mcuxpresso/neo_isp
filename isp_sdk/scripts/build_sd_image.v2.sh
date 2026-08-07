#!/bin/bash
#
# Copyright 2025,2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause
####################################################

# This script builds complete SD card image running application on i.mx95's M7 core

## exit on error
handle_error() {
    echo
    echo ">>> ERROR <<<"
    echo "SD card build - error in $0 on line $1"
    echo "Did you forget to ... ?"
    echo "    source precfg.sh"
    exit -1
}

trap 'handle_error $LINENO' ERR

export ISPSDK_ROOT=$(cd "../"; pwd)

if (($# < 2))
then
    echo "usage : build_sd_image.sh [r,d] [IMX95_A0 | IMX95_B0] <demo_app>  (release/debug, target platform, target demo)"
    exit -1
fi

if [[ "$1" == "r" ]]; then
    export BUILD_TARGET="release"
elif [[ "$1" == "d" ]]; then
    export BUILD_TARGET="debug"
else
    echo "Invalid build type. Use 'r' for release or 'd' for debug."
    exit 1
fi

if (( $# >= 2 ))
then
    export HW_REVISION=$2
    case $HW_REVISION in
        'IMX95_A0')
            echo "A0 not supported, it migh be possible, but APP must be manually switched via its proj.conf"
            exit 1
        ;;
        'IMX95_B0')
            export BOARD_ID=imx95lpd5evk19
            export TARGET_CORE_ID=cm7
            export SOC_REV=B0
        ;;
        *)
            echo "Unknow hw revision "$HW_REVISION" provided."
            exit 1;;
    esac
    export BUILD_TARGET_DIR=$BUILD_TARGET"_"$HW_REVISION
else
    echo "No revision specified."
    exit 1
fi

if [[ $# < 3 ]]; then
    export TARGET_APP_ID="isp_ccl"
    echo "Application not specified, using default:"
    echo $TARGET_APP_ID
else
    export TARGET_APP_ID=$3
fi
export TARGET_APP_BUILD_PATH="examples/demo_apps/$TARGET_APP_ID"
export TARGET_APP_ELF_NAME="${TARGET_APP_ID}_${TARGET_CORE_ID}.elf"
TARGET_OUT_DIR="../../../../../../${TARGET_APP_ID}_${BUILD_TARGET}"
export TARGET_OUT_DIR=$(realpath "$TARGET_OUT_DIR")

if [[ "$TARGET_APP_ID" == "isp_ccl" ]]; then
    export USE_DDR_INIT=1
else
    export USE_DDR_INIT=0
fi


echo
echo ISPSDK repository location: $ISPSDK_ROOT
echo ARM toolchains location: $ARMTOOLS
echo image build tool: $IMG_BUILD_TOOL
echo Target APP: $TARGET_APP_ID
echo Output dir: $TARGET_OUT_DIR
echo Preparing composite with DDR init section: $USE_DDR_INIT
echo

# *** build APP ***
pushd ../../../..
west build ${TARGET_APP_BUILD_PATH} --config ${BUILD_TARGET} -b ${BOARD_ID} --toolchain armgcc -p always -Dcore_id=${TARGET_CORE_ID}  -d ${TARGET_OUT_DIR}

APP_ELF=$TARGET_OUT_DIR/$TARGET_APP_ELF_NAME
export APP_ELF=$(realpath "$APP_ELF")


# **********************************************************
#              Compile APP image
# **********************************************************

#list elf sections
echo Listing ELF sections for ...
echo $APP_ELF
readelf -W -l -t $APP_ELF

# list of code sections to be included in the SD image (may not be used, see actual arm-none-eabi-objcopy commands below)
echo "Extracting code sections into dedicated files ..."
code_sections=(".interrupts" ".text" ".ARM" ".init_array" ".fini_array")
data_sections=(".bss" ".heap" ".stack")
ddr_sections=(".data" ".ncache.init" ".ncache" ".ddr_data.init" ".ddr_data" ".ddr_code.init")
# construct section commands
code_args=""
for item in "${code_sections[@]}"; do
    code_args="${code_args} --only-section=$item"
done
data_args=""
for item in "${data_sections[@]}"; do
    data_args="${data_args} --only-section=$item"
done
ddr_args=""
for item in "${ddr_sections[@]}"; do
    ddr_args="${ddr_args} --only-section=$item"
done


#prepare directories
echo "Preparing input files and output directories..."
OUT_DIR=${TARGET_OUT_DIR}/sd_image
mkdir -p $OUT_DIR 2>/dev/null || true

#*** copy components to staging dir ***

#copy APP
#NOTE at present app_data constants are appended after app_code by liker and expected there by startup script,
# so I do NOT extract app_code sections rather I create full binary image (as there is nothing beyond code address)
if [[ $USE_DDR_INIT == 1 ]]; then
    ${ARMTOOLS}/bin/arm-none-eabi-objcopy $code_args -v -O binary $APP_ELF $OUT_DIR/app_code_only.bin
    ${ARMTOOLS}/bin/arm-none-eabi-objcopy $data_args -v -O binary $APP_ELF $OUT_DIR/app_data.bin
    ${ARMTOOLS}/bin/arm-none-eabi-objcopy $ddr_args -v -O binary $APP_ELF $OUT_DIR/app_ddr.bin
    # Combine code and data sections so startup code can initialize data section properly
    cat $OUT_DIR/app_code_only.bin > $OUT_DIR/app_code.bin
    cat $OUT_DIR/app_data.bin >> $OUT_DIR/app_code.bin
else
    ${ARMTOOLS}/bin/arm-none-eabi-objcopy -v -O binary $APP_ELF $OUT_DIR/app_code.bin
fi

#copy system manager - now prebuild delete this line
#copy some other pre-build components -> bootloaders, DDR init code,...
cp -v ${ISPSDK_ROOT}/scripts/sd_components/app_m7_${SOC_REV}-a55_uboot/* $OUT_DIR/

cp -v ${ISPSDK_ROOT}/scripts/sd_components/spsdk_templates/* $OUT_DIR/

if [[ $USE_DDR_INIT == 1 ]]; then
    mv $OUT_DIR/mx95_uboot_primary_ddr_init.yaml $OUT_DIR/mx95_uboot_primary.yaml
else
    mv $OUT_DIR/mx95_uboot_primary_mtext_init.yaml $OUT_DIR/mx95_uboot_primary.yaml
fi

echo
echo
echo "******  Inputs generated, proceeding with final compilation  ********"

#*** Compile all binaries ***
nxpimage -v bootable-image export -c $OUT_DIR/bootimg_mx95_serial_downloader.yaml -o $OUT_DIR/flash.bin

echo
echo
echo "******  Image ready!  ********"
echo "You can verify what is in the image by running:"
echo "nxpimage -v bootable-image verify -b $OUT_DIR/flash.bin -f mx95 -m serial_downloader"
echo For flashing...
echo USE:
echo "-> BOOTABLE SD card and"
echo "-> in command bellow update target (replace sdb) with proper path. Use ./list_disks.sh to list currently connected drives."
echo "sudo dd if=$OUT_DIR/flash.bin of=/dev/sdb bs=1k seek=32 oflag=direct status=progress"
echo

popd

exit 0
