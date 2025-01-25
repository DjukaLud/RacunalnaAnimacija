extends Control

func _ready() -> void:
	$VBoxContainer/StartButton.grab_focus()

func _on_start_button_pressed() -> void:
	var world_scene = load("res://Scenes/World.tscn").instantiate()
	get_tree().root.add_child(world_scene)
	get_tree().current_scene.queue_free() 
	get_tree().current_scene = world_scene

func _on_quit_button_pressed() -> void:
	get_tree().quit()
