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

    const buttons = document.querySelectorAll('.button');
    buttons.forEach(button => {
        button.addEventListener('click', () => {
            console.log(`${button.id.replace('button-', '')} button pressed`);
        });
    });

    const bumpers = document.querySelectorAll('.bumper');
    bumpers.forEach(bumper => {
        bumper.addEventListener('click', () => {
            console.log(`${bumper.id.replace('bumper-', '')} bumper pressed`);
        });
    });

    const triggers = document.querySelectorAll('.trigger');
    triggers.forEach(trigger => {
        trigger.addEventListener('click', () => {
            console.log(`${trigger.id.replace('trigger-', '')} trigger pressed`);
        });
    });

    const dpadButtons = document.querySelectorAll('.dpad-button');
    dpadButtons.forEach(dpadButton => {
        dpadButton.addEventListener('click', () => {
            console.log(`${dpadButton.id.replace('dpad-', '')} pressed`);
        });
    });

    const minusButton = document.getElementById('minus');
    const plusButton = document.getElementById('plus');
    const squareButton = document.getElementById('square');
    const homeButton = document.getElementById('home');

    minusButton.addEventListener('click', () => {
        console.log('Minus button pressed');
    });

    plusButton.addEventListener('click', () => {
        console.log('Plus button pressed');
    });

    squareButton.addEventListener('click', () => {
        console.log('Square button pressed');
    });

    homeButton.addEventListener('click', () => {
        console.log('Home button pressed');
    });
});
