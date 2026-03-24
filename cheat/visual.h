#pragma once
class visual
{
public:
	static visual* get();
	void init();
	void main();
private:
	void third_person();
	void fov_change();
	void post();
};

