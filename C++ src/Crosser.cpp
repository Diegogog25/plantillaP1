void Crosser::update()
{
    // Avanza
    pos += vel;

    // Mantén el desfase entre objetos al hacer wrap
    const float span = (rightLimit - leftLimit) + w; // ancho de carril + ancho del objeto

    if (vel.getX() > 0) {
        // Hacia la derecha: cuando pase el límite derecho, trasládalo una vuelta completa
        if (pos.getX() > rightLimit) {
            pos.setX(pos.getX() - span); // conserva la fase (espaciado) original
        }
    } else if (vel.getX() < 0) {
        // Hacia la izquierda: cuando el objeto entero pase el límite izquierdo
        if (pos.getX() + w < leftLimit) {
            pos.setX(pos.getX() + span); // conserva la fase (espaciado) original
        }
    }
}