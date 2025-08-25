local projectName = "UnrealEngineBypassMod"

target(projectName)
	add_rules("ue4ss.mod")
	
	add_includedirs(".")
	add_includedirs("include", { public = true })
    add_headerfiles("include/**.hpp")
	
	add_files("**.cpp")
    add_files("src/**.cpp")
	add_files("dllmain.cpp")