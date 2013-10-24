function clone(var)
	local szType = type(var)
	if szType == "nil"
	or szType == "boolean"
	or szType == "number" 
	or szType == "string" then
		return var
	elseif szType == "table" then
		local tTable = {}
		for key, val in pairs(var) do
			key = clone(key)
			val = clone(val)
			tTable[key] = val
		end
		return tTable
	elseif szType == "function" 
	or szType == "userdata" then
		return nil
	else
		return nil
	end	
end

function copyValue(var, var1)
	local szType = type(var)
	if szType == "nil"
	or szType == "boolean"
	or szType == "number" 
	or szType == "string" then
		var = var1
	elseif szType == "table" then
		for key, val in pairs(var) do
            if type(val) == "table" then
                copyValue(var[key], var1[key])
            else
                var[key] = var1[key]
            end
		end
	elseif szType == "function" 
	or szType == "userdata" then
		var = var1
	else
		var = var1
	end	
end

function copyValueEx(var, var1)
	local szType = type(var)
	if szType == "table" then
        local nSize1 = #var1
        local nSize = #var
        if nSize > nSize1 then
            for key, val in pairs(var) do
                if type(val) == "table" then
                    if not var1[key] then
                        var[key] = nil
                    else
                        copyValueEx(var[key], var1[key])
                    end
                else
                    var[key] = var1[key]
                end
            end
        else
            for key, val in pairs(var1) do
                if type(val) == "table" then
                    if not var[key] then
                        var[key] = clone(var1[key])
                    else
                        copyValueEx(var[key], var1[key])
                    end
                else
                    var[key] = var1[key]
                end
            end
        end
	else
		var = var1
	end	
end

function var2str(var, szIndent)
	local szType = type(var)
	if szType == "nil" then
		return "nil"
	elseif szType == "number" then
		return tostring(var)
	elseif szType == "string" then
		return string.format("%q", var)
	elseif szType == "function" then
		local szCode = string.dump(var)
		local arByte = { string.byte(szCode, i, #szCode) }
		szCode	= ""
		for i = 1, #arByte do
			szCode	= szCode..'\\'..arByte[i]
		end
		return 'loadstring("' .. szCode .. '")'
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

function Trace(msg)
    WriteLog(1, msg)
end

function Output(...)
  	local arg = {...}
	arg.n = nil -- param count
	
	local szMsg = var2str(arg)
	if Trace then
		Trace("[UI DEBUG]" .. szMsg)
	end
    Print2Screen(szMsg)
end

function ReloadScripts()
    loadluafile()
end

function ValueProcessNil(value)
    if value == nil then
        return ""
    end
    return value;
end

function SplitString(szText, szSeparators)
    local nStart = 1
    local nLen = #szText
    local tResult = {}   
    while nStart <= nLen do
        local nSt, nEnd = string.find(szText, szSeparators, nStart)
        if nSt and nEnd then
            local szResult = string.sub(szText, nStart, nSt - 1)
            table.insert(tResult, szResult)
            nStart = nEnd +1
        else
            if nStart <= nLen then
                local szResult = string.sub(szText, nStart, nLen)
                table.insert(tResult, szResult)
            end
            nStart = nLen + 1
        end
    end 
    return tResult
end

function FixFloat(fNum, nEPS)
	if not nEPS then
		nEPS = 0
	end
	assert(nEPS >= 0)
	return string.format("%g", string.format("%." .. nEPS .. "f", fNum))
end
