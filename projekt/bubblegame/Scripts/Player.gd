extends CharacterBody3D


var speed
const WALK_SPEED = 4.0
const SPRINT_SPEED = 7.0
const JUMP_VELOCITY = 4.5
const SENSITIVITY = 0.001

var bullet = load("res://Scenes/Bullet.tscn")
var instance

@onready var head = $Head
@onready var camera = $Head/Camera3D
@onready var sling_animation = $Head/Camera3D/Slingshot/AnimationPlayer
@onready var sling_barrel = $Head/Camera3D/Slingshot/RayCast3D
@onready var hit_counter_label = $Head/Camera3D/BubbleCounter
@onready var hit_rect = $Head/Camera3D/ColorRect
@onready var stone_counter_label = $Head/Camera3D/StoneCounter
@onready var hits_label = $"../VBoxContainer/HitsLabel"
@onready var misses_label = $"../VBoxContainer/MissesLabel"
@onready var shots_label = $"../VBoxContainer/TotalShotsLabel"
@onready var accuray_label = $"../VBoxContainer/AccuracyLabel"
@onready var end_display = $"../VBoxContainer"
@onready var no_ammo_label = $"../NoBulletsLabel"

var ended = false
var hit_count = 0
var total_shots = 0
var stone_count = 15

var countdown_time = 100
@onready var timer = $Timer
@onready var timer_display_label = $Head/Camera3D/TimerDisplay

@export var is_game_paused = false
func _input(event):
	if event.is_action_pressed("ui_cancel"):
		if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
			Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
			is_game_paused = true
			ended = true
			self.set_process_input(false)
			self.set_process(false)
		else:
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
			is_game_paused = false
			ended = false
			self.set_process_input(true)
			self.set_process(true)

func _ready():
	update_hit_counter()
	update_stone_counter()
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	add_to_group("Player")
	timer_display_label.text = str(countdown_time)
	timer.start()

func _rectangle_timer():
	hit_rect.visible = true
	await get_tree().create_timer(0.1).timeout
	hit_rect.visible = false

func _stone_pickup():
	stone_count += 15
	update_stone_counter()

func update_hit_counter() -> void:
	hit_counter_label.text = "Hits: " + str(hit_count)

func update_stone_counter() -> void:
	stone_counter_label.text = "Stones: " + str(stone_count)

func _unhandled_input(event):
	if event is InputEventMouseMotion and !ended:
		head.rotate_y(-event.relative.x * SENSITIVITY)
		camera.rotate_x(-event.relative.y * SENSITIVITY)
		camera.rotation.x = clamp(camera.rotation.x, deg_to_rad(-45), deg_to_rad(90))

func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity += get_gravity() * delta

	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = JUMP_VELOCITY
	
	if Input.is_action_pressed("sprint"):
		speed = SPRINT_SPEED
	else:
		speed = WALK_SPEED
		
	var input_dir = Input.get_vector("left", "right", "up", "down")
	var direction = (head.transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	
	if direction:
		velocity.x = direction.x * speed
		velocity.z = direction.z * speed
	else:
		velocity.x = 0.0
		velocity.z = 0.0

	if Input.is_action_pressed("shoot"):
		if !sling_animation.is_playing() and stone_count > 0:
			sling_animation.play("Shoot")
			await sling_animation.animation_finished
			instance = bullet.instantiate()
			instance.position = sling_barrel.global_position
			instance.transform.basis = sling_barrel.global_transform.basis
			get_parent().add_child(instance)
			stone_count -= 1
			update_stone_counter()
			total_shots += 1
		if stone_count == 0:
			no_ammo_label.visible = true
			await get_tree().create_timer(1).timeout
			no_ammo_label.visible = false

	move_and_slide()
	
func _on_timer_timeout() -> void:
	countdown_time -= 1
	if countdown_time < 10 : 
		timer_display_label.add_theme_color_override("font_color", Color(1,0,0))
		
	timer_display_label.text = str(countdown_time)
	
	if countdown_time <= 0:
		timer.stop()
		timer_display_label.text = "TIME'S UP"
		hits_label.text = "Total hits: " + str(hit_count)
		misses_label.text = "Total misses: " + str(total_shots-hit_count)
		shots_label.text = "Total shots: " + str(total_shots)
		var accuracy
		if total_shots == 0:
			accuracy = 0
		accuracy = 1.0*hit_count/total_shots*100
		accuray_label.text = "Accuracy: " + str(round(accuracy)) + "%"
		end_display.visible = true
		self.visible = false
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
		ended = true
		self.set_process_input(false)
		self.set_process(false)
