#ifndef CAMERA_H
#define CAMERA_H

#include <serialization/ISerializable.h>
#include <glm/glm.hpp>

namespace ce {

class Camera: public ISerializable {
    public:
        Camera();
        ~Camera()   = default;

        float       fov() const;
        /// \brief Set the vertical field of view.
        /// Value must be greater than zero!
        /// \return f The vertical field-of-view, specified in degrees.
        void        setFov( float f );

        float       nearPlane() const;
        /// \brief Set the closest visible distance from the camera.
        /// Must be less than the current far plane.
        void        setNearPlane( float near );

        float       farPlane() const;
        /// \brief Set the farthest visible distance from the camera.
        /// Must be greater than the current near plane.
        void        setFarPlane( float far );

        float       viewportAspectRatio() const;
        void        setViewportAspectRatio( float aspect );

        /// \brief Toggle the projection mode between perspective and ortho.
        /// \param isPerspective perspective projection is used when true, ortho projection otherwise.
        void        viewPerspective( bool isPerspective );

        /// \brief Dolly along the the local z-axis.
        void        dolly( float movez );

        /// \brief Orbit around the focal point.
        void        orbit( float movex, float movey );

        /// \brief Translate the camera in the local x and y axis.
        void        translate( float movex, float movey );

        /// \brief Update the camera
        /// In a true ECS, this logic would be owned by the system. The system would handle inputs as well.
        void        update( float dt );

        glm::vec3   forward() const;
        glm::vec3   right() const;
        glm::vec3   up() const;

        glm::mat4   view() const;
        glm::mat4   projection() const;
        glm::mat4   matrix() const; //combined view and projection matrix

        void                    deserialize( ObjectNode::node_ptr )     override;
        ObjectNode::node_ptr    serialize()                             override;

    private:
        float       verticalFov_;
        float       nearPlane_;
        float       farPlane_;
        float       aspectRatio_;
        glm::vec3   position_;
        glm::vec3   targetPosition_;
        glm::vec3   focal_;
        float       radius_;    //the distance from the camera's focal point
        float       targetRadius_;
        float       minRadius_;
        float       maxRadius_;
        float       zoomFactor_;
        float       targetZoomFactor_;
        float       dollySensitivity_;
        float       orbitSensitivity_;
        float       panSensitivity_;
        float       polar_;
        float       azimuth_;
        bool        viewPerspective_;
};

} // namespace ce

#endif // CAMERA_H
