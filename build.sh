echo "Starting build..."

cd src/
make windows_i686 linux_release linux_debug

echo "Creating binary folders..."
cd ..

cp -r windows-binary-skel windows-binary
cp -r windows-binary-all-dlls-skel windows-binary-dll
cp -r linux-binary-skel linux-binary

echo "Copying binaries..."
cp src/release/AapeliBlox.exe windows-binary
cp src/release/AapeliBlox.exe windows-binary-dll
cp src/release/AapeliBlox linux-binary

echo "Compressing binaries..."
cd windows-binary
zip AapeliBlox.zip *
cp AapeliBlox.zip ../
cd ..
rm windows-binary/ -r

cd windows-binary-dll
zip AapeliBlox-dll.zip *
cp AapeliBlox-dll.zip ../
cd ..
rm windows-binary-dll/ -r

cd linux-binary
tar -cvzf AapeliBlox.tar.gz *
cp AapeliBlox.tar.gz ../
cd ..
rm linux-binary/ -r

echo "Done."
