#pragma once
#include <unordered_map>
#include <string>
#include <vector>

enum class value_type
{
	vt_bool,
	vt_int,
	vt_float
};

class mem_value
{
public:
	mem_value() = default;
	mem_value(void* ptr, value_type type);
	std::string to_string() const;
	void to_value(const std::string val);

private:
	void* ptr;
	value_type type;
};

class save
{
public:
	static save* get();
	void save_mem(const char* path);
	void load_mem(const char* path);

private:
	bool create_file(const std::string& path);
	std::vector<std::string> read_file(const std::string& path);
	bool write_file(std::vector<std::string> list, const std::string& path);

public:
	std::unordered_map<std::string, mem_value> value_map;
};

#define REGISTER_SAVE(_name_,_type_) save::get()->value_map[#_name_] = mem_value(&_name_, value_type::vt_##_type_);