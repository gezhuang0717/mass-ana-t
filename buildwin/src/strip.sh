echo "Älä tätä käytä!"
exit

echo "before strip:"
ls -la lakritsi.exe
i586-mingw32msvc-strip lakritsi.exe
echo "after:"
ls -la lakritsi.exe

grep current_version=\ \" ../../src/window1.cc |awk '{for(i=2;i<3;i++) print substr($i,2,10) }' > lakritsi_version
echo "Copying it  jalava:igisol_www/trap/lakritsi/"
rsync -auP lakritsi.exe lakritsi_version ../../*txt tomero@jalava.cc.jyu.fi:igisol_www/trap/lakritsi/
#echo "And also for Viki.. to linuxfs"
#rsync -trvP lakritsi.exe ../../*txt tomero@linuxfs.phys.jyu.fi:ig_www/onlyigisol/trap/data/lakritsiwin/
