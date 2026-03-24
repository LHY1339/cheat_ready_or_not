#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace SDK
{
	class AActor;
	class FName;
}

enum class actor_type
{
	team,
	civilian,
	suspect,
	trap,
	report,
	bomb,
	weapon,
	other,
};

struct actor_info
{
	actor_type type;
	SDK::AActor* actor;
};

class manager
{
public:
	static manager* get();
	void init();
	void main();
private:
	void timer();
	void handle(SDK::AActor* actor);
	void print_dbg();

	SDK::FName to_name(const std::wstring& str);
public:
	std::vector<actor_info> actor_list;
	std::unordered_map<std::string, SDK::FName> bone_list;
};

