#include "cheat_mod.h"
#include "gvalue.h"
#include "gdefine.h"
#include "save.h"

void cheat_mod::save()
{
	//character

	REGISTER_SAVE(gvalue::esp_team.enable, bool);
	REGISTER_SAVE(gvalue::esp_team.name, bool);
	REGISTER_SAVE(gvalue::esp_team.distance, bool);
	REGISTER_SAVE(gvalue::esp_team.status, bool);
	REGISTER_SAVE(gvalue::esp_team.line, bool);
	REGISTER_SAVE(gvalue::esp_team.box, bool);
	REGISTER_SAVE(gvalue::esp_team.skeleton, bool);

	REGISTER_SAVE(gvalue::esp_suspect.enable, bool);
	REGISTER_SAVE(gvalue::esp_suspect.name, bool);
	REGISTER_SAVE(gvalue::esp_suspect.distance, bool);
	REGISTER_SAVE(gvalue::esp_suspect.status, bool);
	REGISTER_SAVE(gvalue::esp_suspect.line, bool);
	REGISTER_SAVE(gvalue::esp_suspect.box, bool);
	REGISTER_SAVE(gvalue::esp_suspect.skeleton, bool);

	REGISTER_SAVE(gvalue::esp_civilian.enable, bool);
	REGISTER_SAVE(gvalue::esp_civilian.name, bool);
	REGISTER_SAVE(gvalue::esp_civilian.distance, bool);
	REGISTER_SAVE(gvalue::esp_civilian.status, bool);
	REGISTER_SAVE(gvalue::esp_civilian.line, bool);
	REGISTER_SAVE(gvalue::esp_civilian.box, bool);
	REGISTER_SAVE(gvalue::esp_civilian.skeleton, bool);

	//item

	REGISTER_SAVE(gvalue::esp_trap.enable, bool);
	REGISTER_SAVE(gvalue::esp_trap.name, bool);
	REGISTER_SAVE(gvalue::esp_trap.distance, bool);
	REGISTER_SAVE(gvalue::esp_trap.status, bool);
	REGISTER_SAVE(gvalue::esp_trap.line, bool);
	REGISTER_SAVE(gvalue::esp_trap.circle, bool);

	REGISTER_SAVE(gvalue::esp_report.enable, bool);
	REGISTER_SAVE(gvalue::esp_report.name, bool);
	REGISTER_SAVE(gvalue::esp_report.distance, bool);
	REGISTER_SAVE(gvalue::esp_report.status, bool);
	REGISTER_SAVE(gvalue::esp_report.line, bool);
	REGISTER_SAVE(gvalue::esp_report.circle, bool);

	REGISTER_SAVE(gvalue::esp_bomb.enable, bool);
	REGISTER_SAVE(gvalue::esp_bomb.name, bool);
	REGISTER_SAVE(gvalue::esp_bomb.distance, bool);
	REGISTER_SAVE(gvalue::esp_bomb.status, bool);
	REGISTER_SAVE(gvalue::esp_bomb.line, bool);
	REGISTER_SAVE(gvalue::esp_bomb.circle, bool);

	REGISTER_SAVE(gvalue::esp_weapon.enable, bool);
	REGISTER_SAVE(gvalue::esp_weapon.name, bool);
	REGISTER_SAVE(gvalue::esp_weapon.distance, bool);
	REGISTER_SAVE(gvalue::esp_weapon.status, bool);
	REGISTER_SAVE(gvalue::esp_weapon.line, bool);
	REGISTER_SAVE(gvalue::esp_weapon.circle, bool);

	REGISTER_SAVE(gvalue::esp_other.enable, bool);
	REGISTER_SAVE(gvalue::esp_other.name, bool);
	REGISTER_SAVE(gvalue::esp_other.distance, bool);
	REGISTER_SAVE(gvalue::esp_other.status, bool);
	REGISTER_SAVE(gvalue::esp_other.line, bool);
	REGISTER_SAVE(gvalue::esp_other.circle, bool);

	//distance

	REGISTER_SAVE(gvalue::esp_distance, float);

	//visual

	REGISTER_SAVE(gvalue::tpp, bool);
	REGISTER_SAVE(gvalue::tpp_camera_x, float);
	REGISTER_SAVE(gvalue::tpp_camera_y, float);
	REGISTER_SAVE(gvalue::tpp_camera_arm, float);
	REGISTER_SAVE(gvalue::tpp_camera_collision, bool);
	REGISTER_SAVE(gvalue::first_fov, float);
	REGISTER_SAVE(gvalue::third_fov, float);
	REGISTER_SAVE(gvalue::disable_post, bool);

	//aim

	REGISTER_SAVE(gvalue::aim_enable, bool);
	REGISTER_SAVE(gvalue::aim_wall_cond, bool);
	REGISTER_SAVE(gvalue::aim_down, bool);
	REGISTER_SAVE(gvalue::aim_use_smooth, bool);
	REGISTER_SAVE(gvalue::aim_smooth, float);
	REGISTER_SAVE(gvalue::aim_use_radius, bool);
	REGISTER_SAVE(gvalue::aim_radius_draw, bool);
	REGISTER_SAVE(gvalue::aim_radius, float);

	REGISTER_SAVE(gvalue::aim_left_button, bool);
	REGISTER_SAVE(gvalue::aim_right_button, bool);
	REGISTER_SAVE(gvalue::aim_side_button, bool);
	REGISTER_SAVE(gvalue::aim_alt, bool);

	REGISTER_SAVE(gvalue::aim_no_recoil, bool);
	REGISTER_SAVE(gvalue::aim_no_delta, bool);
	REGISTER_SAVE(gvalue::aim_inf_bullet, bool);
	REGISTER_SAVE(gvalue::aim_fire_rate, bool);
	REGISTER_SAVE(gvalue::aim_magic_bullet, bool);

	REGISTER_SAVE(gvalue::aim_player, bool);
	REGISTER_SAVE(gvalue::aim_civilian, bool);
	REGISTER_SAVE(gvalue::aim_suspect, bool);

	REGISTER_SAVE(gvalue::aim_head, bool);
	REGISTER_SAVE(gvalue::aim_spine, bool);
	REGISTER_SAVE(gvalue::aim_pelvis, bool);
	REGISTER_SAVE(gvalue::aim_hand, bool);
	REGISTER_SAVE(gvalue::aim_foot, bool);

	REGISTER_SAVE(gvalue::global_speed, float);

	save::get()->load_mem(SAVE_PATH);
}