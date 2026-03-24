#include "save.h"

#include <fstream>
#include <filesystem>

save* save::get()
{
	static save inst;
	return &inst;
}

void save::save_mem(const char* path)
{
    create_file(path);

    std::vector<std::string> list;
    for (const auto& it : value_map)
    {
        std::string line = "";
        line += it.first + " ";
        line += it.second.to_string();
        list.emplace_back(line);
        printf("save : %s\n", line.c_str());
    }

    write_file(list, std::string(path));
}

void save::load_mem(const char* path)
{
    std::vector<std::string> list = read_file(path);
    for (const std::string& line : list)
    {
        auto pos = line.find(' ');
        if (pos == std::string::npos)
        {
            continue;
        }
        std::string left = line.substr(0, pos);
        printf("load : %s\n", left.c_str());
        std::string right = line.substr(pos + 1);

        auto it = value_map.find(left);
        if (it != value_map.end())
        {
            it->second.to_value(right);
        }
    }
}

bool save::create_file(const std::string& path)
{
    std::filesystem::path path_obj(path);
    if (std::filesystem::exists(path_obj) && std::filesystem::is_regular_file(path_obj))
    {
        return false;
    }

    std::filesystem::path dir = path_obj.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir))
    {
        std::filesystem::create_directories(dir);
    }

    std::ofstream ofs(path);
    return ofs.good();
}

std::vector<std::string> save::read_file(const std::string& path)
{
    std::vector<std::string> result;
    std::ifstream ifs(path);

    if (!ifs.is_open())
    {
        return result;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        result.push_back(line);
    }

    return result;
}

bool save::write_file(std::vector<std::string> list, const std::string& path)
{
    std::ofstream ofs(path);

    if (!ofs.is_open())
    {
        return false;
    }

    for (const auto& str : list)
    {
        ofs << str << "\n";
    }

    return true;
}

mem_value::mem_value(void* ptr, value_type type)
{
	this->ptr = ptr;
	this->type = type;
}

std::string mem_value::to_string() const
{
    switch (type)
    {
    case value_type::vt_bool:
        return (*(bool*)ptr) ? "true" : "false";
    case value_type::vt_int:
        return std::to_string(*(int*)ptr);
    case value_type::vt_float:
        return std::to_string(*(float*)ptr);
    }
    return "error";
}

void mem_value::to_value(const std::string val)
{
    switch (type)
    {
    case value_type::vt_bool:
        *((bool*)ptr) = val == "true";
        break;
    case value_type::vt_int:
        *((int*)ptr) = std::stoi(val);
        break;
    case value_type::vt_float:
        *((float*)ptr) = std::stof(val);
        break;
    }
}
