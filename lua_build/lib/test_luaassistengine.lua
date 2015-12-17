dofile("D:\\GitHub\\code\\lua_build\\lib\\debug.lua")

importdir("E:\\lua_build\\lib")
local dll = importdll("luaassistengine.dll")

--dll.libinfo()
--print( dll.libinfo() )

-------- ini test --------------------------------

local iniRead = dll.OpenIni("i:\\kingsoft\\sword3-products\\trunk\\client\\config.ini")
local value = iniRead:GetInt("UIEditor", "Enable");
print("GetInt( UIEditor, Enable )", value)

value = iniRead:GetString("UIEditor", "Path");
print("GetString(UIEditor, Path)", value)

value = iniRead:GetFloat("Main", "X");
print("GetFloat(Main, X)", value)

value = iniRead:GetDouble("Main", "Y");
print("GetDouble(Main, Y)", value)

value = iniRead:SectionCount();
print("SectionCount()", value)

value = iniRead:NextSection("UIEditor");
print("NextSection(UIEditor)", value)

value = iniRead:NextSection("JX3Profile");
print("NextSection(JX3Profile)", value)

value = iniRead:NextSection("ParticleEditor");
print("NextSection(ParticleEditor)", value)
dll.CloseIni(iniRead)
iniRead = nil


local ini = dll.OpenIni("i:\\kingsoft\\sword3-products\\trunk\\client\\config.ini", "w")
ini:RemoveSection("Mouse")
ini:Save("i:\\kingsoft\\sword3-products\\trunk\\client\\config1.ini")

ini:WriteInt("Mouse", "SwapMouseButton", 0)
ini:WriteFloat("Mouse", "SwapMouse1", 0.0300)
ini:WriteDouble("Mouse", "SwapMouse2", 1.00000300)
ini:WriteString("Mouse", "SwapMouse3", "good job")

ini:Save("i:\\kingsoft\\sword3-products\\trunk\\client\\config2.ini")
dll.CloseIni(ini)
ini = nil

ini = dll.CreateIni()
ini:WriteString("TestCreate", "create", "true")
ini:Save("i:\\kingsoft\\sword3-products\\trunk\\client\\config3.ini")
dll.CloseIni(ini)
ini = nil


--------- path lib test --------------------------------
--[[
  ["path"] = 
  {
	[1] = "isdir",
	[2] = "isexist",
	[3] = "removespec",
	[4] = "makedir",
	[5] = "cat",
	[6] = "walkdir",
	}
]]
		
print("path.isdir( i:\\kingsoft\\sword3-products\\trunk\\client )",
dll.path.isdir("i:\\kingsoft\\sword3-products\\trunk\\client"))	-- dir

print("path.isdir(i:\\kingsoft\\sword3-products\\trunk\\client\\config3.ini)", 
dll.path.isdir("i:\\kingsoft\\sword3-products\\trunk\\client\\config3.ini")) -- exist file

print("path.isdir(i:\\kingsoft\\sword3-products\\trunk\\client\\aaaaaaa.ini)", 
dll.path.isdir("i:\\kingsoft\\sword3-products\\trunk\\client\\aaaaaaa.ini")) -- not exist file

print("path.isdir(i:\\kingsoft\\sword3-products\\trunk\\clientaaa)", 
dll.path.isdir("i:\\kingsoft\\sword3-products\\trunk\\clientaaa")) -- not exist dir

print("path.isexist(i:\\kingsoft\\sword3-products\\trunk\\client)", 
dll.path.isexist("i:\\kingsoft\\sword3-products\\trunk\\client"))	-- dir

print("path.isexist(i:\\kingsoft\\sword3-products\\trunk\\client\\config3.ini)", 
dll.path.isexist("i:\\kingsoft\\sword3-products\\trunk\\client\\config3.ini")) -- exist file

print("path.isexist(i:\\kingsoft\\sword3-products\\trunk\\client\\aaaaaaa.ini)", 
dll.path.isexist("i:\\kingsoft\\sword3-products\\trunk\\client\\aaaaaaa.ini")) -- not exist file

print("path.isexist(i:\\kingsoft\\sword3-products\\trunk\\clientaaa)", 
dll.path.isexist("i:\\kingsoft\\sword3-products\\trunk\\clientaaa")) -- not exist dir

print("path.removespec(i:\\kingsoft\\sword3-products\\trunk\\client)", 
dll.path.removespec("i:\\kingsoft\\sword3-products\\trunk\\client"))	

print("path.removespec(i:\\kingsoft\\sword3-products\\trunk\\client\\)", 
dll.path.removespec("i:\\kingsoft\\sword3-products\\trunk\\client\\")) 
print("path.removespec(i:\\kingsoft\\sword3-products\\trunk\\client/)",
 dll.path.removespec("i:\\kingsoft\\sword3-products\\trunk\\client/")) 
print("path.removespec(i:\\)", dll.path.removespec("i:\\")) 

print("path.makedir(e:\\a\\b\\c\\d)", dll.path.makedir("e:\\a\\b\\c\\d"))
print("path.makedir(i:\\)", dll.path.makedir("i:\\")) 
print("path.cat(i:\\aa, bbb)", dll.path.cat("i:\\aa", "bbb")) 
print("path.cat(i:\\aa\\, bbb)", dll.path.cat("i:\\aa\\", "bbb")) 
print("path.cat(i:\\aa\\, \\bbb)", dll.path.cat("i:\\aa\\", "\\bbb")) 
print("path.cat(i:\\aa/, bbb)", dll.path.cat("i:\\aa/", "bbb")) 
print("path.cat(i:\\aa/, \\bbb)", dll.path.cat("i:\\aa/", "\\bbb")) 
print("path.cat(i:\\aa\\, /bbb)", dll.path.cat("i:\\aa\\", "/bbb")) 

function task(filepath, info)
	print(filepath, info.bDir)
	if info.bDir then
		return false
	end
	
end

dll.path.walkdir("E:\\lua_build\\lib", task)
--dll.path.walkdir("E:\\lua_build\\", task, "*.exe")
