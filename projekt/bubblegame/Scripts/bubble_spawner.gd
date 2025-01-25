extends Node3D

var bubble_scene = preload("res://Scenes/Bubble.tscn")
const SPAWN_AREA_SIZE = Vector3(10, 7, 10) 
const BUBBLE_COUNT = 2

func _ready() -> void:
	spawn_bubbles(BUBBLE_COUNT)

func spawn_bubbles(count) -> void:
	for i in range(count):
		spawn_bubble()

func spawn_bubble() -> void:
	if bubble_scene:
		var bubble_instance = bubble_scene.instantiate()
		bubble_instance.position = Vector3(
			randf_range(-SPAWN_AREA_SIZE.x, SPAWN_AREA_SIZE.x),
			randf_range(0, SPAWN_AREA_SIZE.y),
			randf_range(-SPAWN_AREA_SIZE.z, SPAWN_AREA_SIZE.z)
		)
		add_child(bubble_instance)


func _on_timer_timeout() -> void:
	spawn_bubbles(BUBBLE_COUNT)
