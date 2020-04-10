echo "before strip:"
ls -la .libs/lakritsi.exe
i586-mingw32msvc-strip .libs/lakritsi.exe
echo "after:"
ls -la .libs/lakritsi.exe

version_number=`grep current_version=\ \" ../../src/window1.cc |awk '{for(i=6;i<7;i++) print substr($i,1,3) }'`
pre="lakritsi_"
aft=".exe"
cp .libs/lakritsi.exe "${pre}${version_number}${aft}"


grep current_version=\ \" ../../src/window1.cc |awk '{for(i=2;i<3;i++) print substr($i,2,10) }' > lakritsi_version
echo "Copying it  jalava:igisol_www/trap/lakritsi/"
rsync -auP "${pre}${version_number}${aft}" lakritsi_version ../../*txt ../../src/ref_masses.txt tomero@jalava.cc.jyu.fi:igisol_www/trap/lakritsi/
#echo "And also for Viki.. to linuxfs"
#rsync -trvP lakritsi.exe ../../*txt tomero@linuxfs.phys.jyu.fi:ig_www/onlyigisol/trap/data/lakritsiwin/
