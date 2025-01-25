extends Area3D

@onready var player = get_tree().root.get_node("World/Player")
@onready var particles = $GPUParticles3D
@onready var mesh = $MeshInstance3D
var velocity = Vector3.ZERO
var speed

func _ready() -> void:
	speed = randf_range(0.1, 3)
	velocity = Vector3(
		randf_range(-1.0, 1.0),
		randf_range(-1.0, 1.0),
		randf_range(-1.0, 1.0)
	).normalized() * speed

func _process(delta: float) -> void:
	position += velocity * delta

func _on_body_entered(body: Node3D) -> void:
	if body.is_in_group("Bullet"):
		_on_bubble_shot()
	else:
		mesh.visible = false
		particles.emitting = true
		await get_tree().create_timer(1.0).timeout
		queue_free()
		
func _on_bubble_shot() -> void:
	player.hit_count += 1
	player.update_hit_counter()
	mesh.visible = false
	particles.emitting = true
	player._rectangle_timer()
	await get_tree().create_timer(1.0).timeout
	queue_free()
	return

func _on_timer_timeout() -> void:
	mesh.visible = false
	particles.emitting = true
	await get_tree().create_timer(1.0).timeout
	queue_free()
