# skyrim-classic-re
My Skyrim Classic reverse-engineering in the form of a VS2015 Shared Items Project.

To use this, add the shared items project to your solution; then, modify the References on your including project so that it actually pulls from the shared items project. Once a Reference is set up, the including project will behave as if the SIP files are in its project folder.

This project contains multiple files that use the same name but are in different folders. The usual fix to prevent this from breaking the MSVC linker is to use an Object File path of *$(IntDir)/%(RelativeDir)/*; however, that's broken for shared items projects. You'll need to use *$(IntDir)/%(Directory)/* instead.