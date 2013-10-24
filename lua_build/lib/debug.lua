function var2str(var, szIndent)
	local szType = type(var)
	if szType == "nil" then
		return "nil"
	elseif szType == "number" then
		return tostring(var)
	elseif szType == "string" then
		return string.format("%q", var)
	elseif szType == "function" then
		return tostring(var)
	elseif szType == "table" then
		if not szIndent then
			szIndent = ""
		end
		
		local szTbBlank	= szIndent .. "  "
		local szCode = ""
		
		for key, val in pairs(var) do
			local szPair = szTbBlank .. "[" .. var2str(key) .. "] = " .. var2str(val, szTbBlank) .. ",\n"
			szCode = szCode .. szPair
		end
		
		if (szCode == "") then
			return "{}"
		else
			return "\n"..szIndent.."{\n"..szCode..szIndent.."}"
		end
	elseif szType == "boolean" then
		return tostring(var)
	elseif (szType == "userdata") then
		return '"' .. tostring(var) .. '"'
	end
end


local function var2strEx(var, szIndent, layer)
	layer = layer or 1
	local szType = type(var)
	if szType == "nil" then
		return "nil"
	elseif szType == "number" then
		return tostring(var)
	elseif szType == "string" then
		return string.format("%q", var)
	elseif szType == "function" then
		return tostring(var)
	elseif szType == "table" then
		if not szIndent then
			szIndent = ""
		end
		
		local szTbBlank	= szIndent .. "  "
		local szCode = ""
		
		for key, val in pairs(var) do
			local szPair = ""
			if layer == 1 then
				szPair = var2str(val, szTbBlank, layer + 1) .. "\n"
			else
				szPair = szTbBlank .. "[" .. var2str(key, nil, layer + 1) .. "] = " .. var2str(val, szTbBlank, layer + 1) .. ",\n"
			end
			
			szCode = szCode .. szPair
		end
		
		if layer == 1 then
			return szCode
		end
		
		if (szCode == "") then
			return "{}"
		else
			return "\n"..szIndent.."{\n"..szCode..szIndent.."}"
		end
	elseif szType == "boolean" then
		return tostring(var)
	elseif (szType == "userdata") then
		return '"' .. tostring(var) .. '"'
	end
end

local _print = print
print = function(...)
	local args = {...}
	_print(var2strEx(args))
end

--print(1,2,57)