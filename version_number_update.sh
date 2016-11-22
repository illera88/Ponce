#script to update the version with the last commit number
MAIN_VERSION_NUMBER="0.2"
DATE=`date +"%Y%m%d"`
COMMIT=`git rev-parse HEAD|cut -c 1-7`
FINAL_VERSION="$MAIN_VERSION_NUMBER.$DATE.$COMMIT"
FILE_CONTENT="#define VERSION \""$FINAL_VERSION"\""
CURRENT_CONTENT=`cat VERSION_NUMBER`
#If the content file is the same we don't overwrite it so the compiler don't need to recompile the whole project
if [ "$FILE_CONTENT" != "$CURRENT_CONTENT" ]
then
	
	echo $FILE_CONTENT > VERSION_NUMBER
fi
