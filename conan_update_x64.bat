set current_path=%cd%


set conan_dir=%current_path%\.main_cache\conan


conan install conanfile.txt -if %conan_dir%\x64\Debug   -s build_type=Debug   --build=missing
conan install conanfile.txt -if %conan_dir%\x64\Release -s build_type=Release --build=missing