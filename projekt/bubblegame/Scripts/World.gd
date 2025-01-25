extends Node3D


func _on_continue_button_pressed() -> void:
	var scene = preload("res://Scenes/MainMenu.tscn").instantiate()
	get_tree().root.add_child(scene)
	get_tree().current_scene.queue_free()
	get_tree().current_scene = scene
