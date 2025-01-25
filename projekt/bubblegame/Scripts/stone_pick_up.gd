extends Node3D

@onready var player = get_tree().root.get_node("World/Player")

func _on_area_3d_body_entered(body: Node3D) -> void:
	if body.is_in_group("Player"):
		player._stone_pickup()
		queue_free()
	
	
