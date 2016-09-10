For every version of IDa you need to check the correspondant blog entry. For 6.95 http://www.hexblog.com/?p=1051.

Need to download the source code from the link they provide in the blog post.
Download the patch too.

Apply the patch: 
 - pip install patch
 - ```C:\Python27>python "C:\Python27\Lib\site-packages\patch.py" -v -d "C:\Users\default.default-pc\Downloads\qt-everywhere-opensource-src-5.6.0\qt-everywhere-opensource-src-5.6.0" "C:\Users\default.default-pc\Downloads\qt-5_6_0_full.patch"```
 
Open the VS2015 x86 console and run the command provided in the hexrays blog. (```configure.bat "-nomake" "tests" "-qtnamespace" "QT" "-confirm-license" "-accessibility" "-opensource" "-force-debug-info" "-platform" "win32-msvc2015" "-opengl" "desktop" "-prefix" "C:/Qt/5.6.0"```)
