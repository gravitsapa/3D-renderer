#include <interactive_processor.h>

namespace project {
namespace application {

namespace detail {

geometry::Coordinate InteractiveProcessor::ConvertToGeometryCoordinate(int coord) {
    return static_cast<geometry::Coordinate>(coord);
}

geometry::Rotation InteractiveProcessor::MoveRotationByMouseDisplacement(
    geometry::Rotation rotation, sf::Vector2i displacement) {
    static constexpr int pixels_per_radian = 300;
    rotation.rot_matrix = geometry::Rotation::ByAngles(
                              -ConvertToGeometryCoordinate(displacement.x) / pixels_per_radian, 0,
                              ConvertToGeometryCoordinate(displacement.y) / pixels_per_radian)
                              .rot_matrix *
                          rotation.rot_matrix;

    return rotation;
}

geometry::Position InteractiveProcessor::MovePositionByArrows(geometry::Position position,
                                                              bool left_holded, bool right_holded,
                                                              bool up_holded, bool down_holded,
                                                              bool forward_holded, bool back_holded,
                                                              double delta_time) {
    static constexpr geometry::Coordinate distance_per_second = 0.1;
    geometry::Coordinate distance_to_move = distance_per_second * delta_time;
    if (left_holded && !right_holded) {
        position.pos_vector.x() -= distance_to_move;
    } else if (right_holded && !left_holded) {
        position.pos_vector.x() += distance_to_move;
    }

    if (down_holded && !up_holded) {
        position.pos_vector.y() -= distance_to_move;
    } else if (up_holded && !down_holded) {
        position.pos_vector.y() += distance_to_move;
    }

    if (forward_holded && !back_holded) {
        position.pos_vector.z() -= distance_to_move;
    } else if (back_holded && !forward_holded) {
        position.pos_vector.z() += distance_to_move;
    }

    return position;
}

const kernel::PosedCamera& InteractiveProcessor::operator()(kernel::World& world) {
    mouse_tracker_.Frame();
    left_arrow_tracker_.Frame();
    right_arrow_tracker_.Frame();
    up_arrow_tracker_.Frame();
    down_arrow_tracker_.Frame();
    w_tracker_.Frame();
    s_tracker_.Frame();
    double delta_time = delta_timer_.TimeInSeconds();
    delta_timer_.Reset();

    auto showing_pose_now = geometry::Pose(world.GetObject(0));

    geometry::Rotation new_rot;
    if (mouse_tracker_.IsLeftButtonHolded()) {
        new_rot = MoveRotationByMouseDisplacement(real_object_rotation_,
                                                  mouse_tracker_.DisplacementWhileLeftHolded());
    } else {
        if (mouse_tracker_.IsLeftButtonJustReleased()) {
            new_rot = MoveRotationByMouseDisplacement(real_object_rotation_,
                                                      mouse_tracker_.DisplacementWhileLeftHolded());
        } else {
            new_rot = showing_pose_now;
        }
        real_object_rotation_ = new_rot;
    }

    geometry::Position new_position = MovePositionByArrows(
        showing_pose_now, left_arrow_tracker_.IsHolded(), right_arrow_tracker_.IsHolded(),
        up_arrow_tracker_.IsHolded(), down_arrow_tracker_.IsHolded(), w_tracker_.IsHolded(),
        s_tracker_.IsHolded(), delta_time);

    world.MoveObject(0, {new_rot, new_position});

    return world.GetCamera(0);
}

}  // namespace detail
}  // namespace application
}  // namespace project
