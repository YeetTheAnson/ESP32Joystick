document.addEventListener('DOMContentLoaded', () => {
    const joystick1 = document.getElementById('joystick1');
    const stick1 = document.getElementById('stick1');
    const joystick2 = document.getElementById('joystick2');
    const stick2 = document.getElementById('stick2');

    let isDragging = false;
    let currentStick = null;

    function updateStickPosition(e, joystick, stick) {
        const rect = joystick.getBoundingClientRect();
        const joystickCenterX = rect.left + rect.width / 2;
        const joystickCenterY = rect.top + rect.height / 2;
        const maxDistance = rect.width / 2;

        const x = e.clientX - joystickCenterX;
        const y = e.clientY - joystickCenterY;

        const distance = Math.min(Math.sqrt(x * x + y * y), maxDistance);
        const angle = Math.atan2(y, x);

        const stickX = distance * Math.cos(angle);
        const stickY = distance * Math.sin(angle);

        stick.style.transform = `translate(${stickX}px, ${stickY}px)`;

        console.log(`Joystick ${joystick.id} - X: ${stickX.toFixed(2)}, Y: ${stickY.toFixed(2)}`);
    }

    function resetStick(stick) {
        stick.style.transform = `translate(0, 0)`;
    }

    function onMouseMove(e) {
        if (isDragging && currentStick) {
            updateStickPosition(e, currentStick.joystick, currentStick.stick);
        }
    }

    function onMouseDown(e) {
        if (e.target.classList.contains('stick')) {
            isDragging = true;
            currentStick = {
                joystick: e.target.parentElement,
                stick: e.target
            };
            updateStickPosition(e, currentStick.joystick, currentStick.stick);
        }
    }

    function onMouseUp() {
        if (isDragging && currentStick) {
            resetStick(currentStick.stick);
            console.log(`Joystick ${currentStick.joystick.id} - X: 0, Y: 0`);
            currentStick = null;
        }
        isDragging = false;
    }

    document.addEventListener('mousemove', onMouseMove);
    document.addEventListener('mousedown', onMouseDown);
    document.addEventListener('mouseup', onMouseUp);
});