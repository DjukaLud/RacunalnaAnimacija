extends VBoxContainer

@onready var player = load("res://Scenes/Player.tscn")

func _ready() -> void:
	$OneMinButton.grab_focus()
	
	
func _on_one_min_button_pressed() -> void:
	player.countdown_time = 60


func _on_two_min_button_pressed() -> void:
	player.countdown_time = 120


func _on_five_min_button_pressed() -> void:
	player.countdown_time = 300


func _on_back_button_pressed() -> void:
	var scene = load("res://Scenes/MainMenu.tscn").instantiate()
	get_tree().root.add_child(scene) 
	get_tree().current_scene.queue_free()
	get_tree().current_scene = scene
