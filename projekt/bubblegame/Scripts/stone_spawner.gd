extends Node3D

var stone_pickup_scene = preload("res://Scenes/Stone_pick_up.tscn")
var parachute_scene = preload("res://Scenes/Parachute.tscn")
const SPAWN_AREA_SIZE = Vector3(8, 0, 8) 
const PICKUP_COUNT = 1
const FALL_SPEED = 2

var parachutes = []

func spawn_stones(count) -> void:
	for i in range(count):
		spawn_stone()

func spawn_stone() -> void:
	if parachute_scene:
		var parachute_instance = parachute_scene.instantiate()
		
		parachute_instance.position = Vector3(
			randf_range(-SPAWN_AREA_SIZE.x, SPAWN_AREA_SIZE.x),
			10.0,
			randf_range(-SPAWN_AREA_SIZE.z, SPAWN_AREA_SIZE.z)
		)
		add_child(parachute_instance)
		
		parachutes.append(parachute_instance)

func _process(delta) -> void:
	for parachute_instance in parachutes:
		parachute_instance.position.y -= FALL_SPEED * delta

		if parachute_instance.position.y <= 0:
			_on_parachute_land(parachute_instance)

func _on_parachute_land(parachute_instance) -> void:
	var new_stone_instance = stone_pickup_scene.instantiate()
	new_stone_instance.position = parachute_instance.position
	new_stone_instance.rotation = parachute_instance.rotation

	parachute_instance.queue_free()
	add_child(new_stone_instance)

	parachutes.erase(parachute_instance)

func _on_timer_timeout() -> void:
	spawn_stones(PICKUP_COUNT)
