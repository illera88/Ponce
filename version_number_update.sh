#script to update the version with the last commit number
MAIN_VERSION_NUMBER="0.1"
DATE=`date +"%Y%m%d"`
COMMIT=`git rev-parse HEAD|cut -c 1-7`
FINAL_VERSION="$MAIN_VERSION_NUMBER.$DATE.$COMMIT"
echo "#define VERSION \""$FINAL_VERSION"\"" > VERSION_NUMBER
