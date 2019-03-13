# skyrim-classic-re
My Skyrim Classic reverse-engineering in the form of a VS2015 Shared Items Project -- a way to hopefully avoid having to bundle separate copies of all of my findings with every mod I release. I'll be updating the existing repos to rely on this as I find the time.

To use this, add the shared items project to your solution; then, modify the References on your including project so that it actually pulls from the shared items project. Once a Reference is set up, the including project will behave as if the SIP files are in its project folder. *Alternatively*, you may find it easier to literally just copy the full contents of this repo into whatever project you're working on. Include paths should work the same either way.

This project contains multiple files that use the same name but are in different folders. The usual fix to prevent this from breaking the MSVC linker is to use an Object File path of *$(IntDir)/%(RelativeDir)/*; however, that's hilariously broken for shared items projects and will cause Visual Studio to construct invalid paths that the linker will choke on. An alternative is to use *$(IntDir)/%(Directory)/* instead... which in turn puts you at risk of MSVC [choking on long file paths](https://stackoverflow.com/questions/49603759/msb6003-the-specified-task-executable-cl-exe-could-not-be-run). If you want to use this as a shared items project, instead of just copying the files into each project you make that uses it, then the workaround for these two bugs is... [ugly](https://github.com/DavidJCobb/skyrim-classic-fixes/blob/master/plugin/CobbBugFixes/CobbBugFixes.vcxproj). The solution is to embed .NET code into the (poorly-documented) MSBuild XML, in order to programmatically adjust \*.OBJ file paths on a per-\*.CPP-file basis immediately before a build takes place.

The benefits of splitting all of this into a shared items project do outweigh the costs, and those costs aren't *my* fault, but it's so kludgey that I kind of feel the need to apologize anyway. :\

----

This project relies on some SKSE source files in order to function. Some class definitions are based on SKSE class definitions. I do not place any limits of my own on using the contents of this repo, but anyone seeking to use this code should be mindful and respectful of the SKSE team and their work.
