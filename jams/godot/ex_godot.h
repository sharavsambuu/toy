//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <godot/Forward.h>
#include <toy/toy.h>

using namespace mud;
using namespace toy;

extern "C"
{
	//_GODOT_EXPORT void ex_godot_game(GameShell& app, Game& game);
}

namespace mud
{
	template <> struct TypedBuffer<Bullet> { static size_t index() { return 20; } };
	template <> struct TypedBuffer<Human>  { static size_t index() { return 21; } };
	template <> struct TypedBuffer<Lamp>   { static size_t index() { return 22; } };
	template <> struct TypedBuffer<Crate>  { static size_t index() { return 23; } };
	template <> struct TypedBuffer<WorldBlock>  { static size_t index() { return 24; } };
}

using HBullet = ComponentHandle<Bullet>;
using HHuman = ComponentHandle<Human>;
using HLamp = ComponentHandle<Lamp>;
using HCrate = ComponentHandle<Crate>;

class refl_ _GODOT_EXPORT Bullet
{
public:
	Bullet() {}
	Bullet(HSpatial spatial, const vec3& source, const quat& rotation, float velocity);

	static Entity create(ECS& ecs, HSpatial parent, const vec3& source, const quat& rotation, float velocity);

	comp_ HSpatial m_spatial;

	attr_ vec3 m_source;
	attr_ vec3 m_velocity;

	bool m_impacted = false;
	bool m_destroy = false;
	vec3 m_impact = Zero3;

	//OSolid m_solid;
	OCollider m_collider;

	void update();
};

enum class refl_ Faction
{
	Ally,
	Enemy
};

struct refl_ Aim
{
	attr_ quat rotation;
	attr_ vec3 start;
	attr_ vec3 end;
	attr_ Spatial* hit;
};

struct HumanController
{
	vec3 m_force = Zero3;
	vec3 m_torque = Zero3;
};

struct refl_ Stance
{
	constr_ Stance() : name(""), loop(false) {}
	constr_ Stance(const std::string& name, bool loop) : name(name), loop(loop) {}
	attr_ std::string name;
	attr_ bool loop;
};

class refl_ _GODOT_EXPORT Human
{
public:
	constr_ Human() {}
	constr_ Human(HSpatial spatial, HMovable movable, HEmitter emitter, HReceptor receptor, HEntityScript script, Faction faction);

	static Entity create(ECS& ecs, HSpatial parent, const vec3& position, Faction faction);

	comp_ HSpatial m_spatial;
	comp_ HMovable m_movable;
	comp_ HEmitter m_emitter;
	comp_ HReceptor m_receptor;
	comp_ HEntityScript m_script; // @todo ---->> ECS

	OSolid m_solid;

	attr_ Faction m_faction;

	vec2 m_angles = Zero2;
	bool m_aiming = false;
	Aim m_visor;

	attr_ float m_life = 1.f;
	attr_ float m_energy = 1.f;
	attr_ float m_discharge = 0.f;

	attr_ bool m_headlight = true;
	attr_ bool m_shield = false;
	attr_ bool m_walk = true;

	bool m_stealth = false;

	attr_ Human* m_target = nullptr;
	attr_ vec3 m_dest = Zero3;
	attr_ float m_cooldown = 0.f;

	attr_ Stance m_state = { "IdleAim", true };

	std::vector<EntityHandle<Bullet>> m_bullets;

	void next_frame(Spatial& spatial, Movable& movable, Receptor& receptor, size_t tick, size_t delta);

	meth_ quat sight(bool aiming = true);
	meth_ Aim aim();
	meth_ void shoot();
	meth_ void stop();
	void damage(float amount);

	static const vec3 muzzle_offset;
	static float headlight_angle;
};

class refl_ _GODOT_EXPORT Lamp
{
public:
	constr_ Lamp() {}
	constr_ Lamp(HSpatial spatial, HMovable movable);

	static Entity create(ECS& ecs, HSpatial parent, const vec3& position);

	comp_ HSpatial m_spatial;
	comp_ HMovable m_movable;
};

class refl_ _GODOT_EXPORT Crate
{
public:
	constr_ Crate() {}
	constr_ Crate(HSpatial spatial, HMovable movable, const vec3& extents);

	static Entity create(ECS& ecs, HSpatial parent, const vec3& position, const vec3& extents);

	comp_ HSpatial m_spatial;
	comp_ HMovable m_movable;

	attr_ vec3 m_extents;
	OSolid m_solid;
};

class refl_ _GODOT_EXPORT WorldBlock
{
public:
	constr_ WorldBlock() {}
	constr_ WorldBlock(HSpatial spatial, HWorldPage world_page, HNavblock navblock, const vec3& extents);

	static Entity create(ECS& ecs, HSpatial parent, const vec3& position, const vec3& extents);

	comp_ HSpatial m_spatial;
	comp_ HWorldPage m_world_page;
	comp_ HNavblock m_navblock;

	attr_ vec3 m_extents;
};

class refl_ _GODOT_EXPORT Player
{
public:
	Player(DefaultWorld& world);
	
	void spawn(const vec3& start_position);
	
	DefaultWorld* m_world;
	HHuman m_human = {};
	Viewer* m_viewer = nullptr;
	ui::OrbitMode m_mode = ui::OrbitMode::ThirdPerson;
};
