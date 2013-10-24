-- 系统目录操作接口 
--[[
lock
chdir
touch
lock_dir
attributes
unlock
link
currentdir
mkdir
symlinkattributes
dir
rmdir
setmode
]]

importdir("E:\\lua_build\\lib") -- ex dll dir
importdll("lfs.dll")	-- load dll 

print("lfs fun list:")
for k, v in pairs(lfs) do
	if type(v) == "function" then
		print(k)
	end
end
