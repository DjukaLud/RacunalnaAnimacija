extends Node3D

const SPEED = 25.0
const GRAVITY = -9.8

@onready var mesh = $MeshInstance3D
@onready var ray = $RayCast3D

var velocity: Vector3

func _ready() -> void:
	add_to_group("Bullet")
	var shoot_direction = global_transform.basis.x.normalized()
	velocity = shoot_direction * SPEED
	
func _physics_process(delta: float) -> void:
	velocity.y += GRAVITY * delta
	
	ray.global_transform.origin = global_transform.origin
	position += velocity * delta

func _on_timer_timeout() -> void:
	queue_free()
