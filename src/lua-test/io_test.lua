-- =========================================
-- RegenX IO Test Script (Lua)
-- Purpose: Validate all file IO operations
-- =========================================

local path = "rx_lua_io_test.txt"

-- cleanup from previous runs
if file.exists(path) then
	print("[Lua] File already exists, deleting:", path)
	file.delete(path)
end

-- create file with initial content
print("[Lua] Creating file:", path)
file.create(path,
	[[RegenXIO LUA TEST
	Line 1: File created successfully
	Line 2: Initial content written
	]])

-- append content
print("[Lua] Appending content")
file.write(path, "Line 3: Appended content\n")

-- overwrite file
print("[Lua] Overwriting file")
file.overwrite(path,
	[[RegenXIO LUA TEST (OVERWRITE)
	Line 1: File overwritten
	Line 2: New content active
	]])

-- append again after overwrite
file.write(path, "Line 3: Append after overwrite\n")

-- read entire file
print("[Lua] Reading full file:")
local full = file.read(path)
print("----- FILE CONTENT START -----")
print(full)
print("----- FILE CONTENT END -----")

-- read specific line (0-based index assumed)
print("[Lua] Reading line 1:")
local line1 = file.read_line(path, 1)
print("Line 1 =", line1)

-- final cleanup
print("[Lua] Deleting file:", path)
file.delete(path)

print("[Lua] IO test completed successfully")
