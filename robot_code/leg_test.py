from inverse_kinematics import calc_angles
import moteus
import asyncio

async def main():
    transport = moteus_pi3hat.Pi3HatRouter(
        servo_bus_map={
            1: [1, 2]
        },
    )

    upper_controller = moteus.Controller(id=1, transport=transport)
    lower_controller = moteus.Controller(id=2, transport=transport)
    await upper_controller.set_stop()
    await lower_controller.set_stop()

    a, b = calc_angles(0, 5)

    result1 = await upper_controller.set_position(position=a, accel_limit=5.0)
    result2 = await lower_controller.set_position(position=b, accel_limit=5.0)
    print(result)


if __name__=="__main__":
    asyncio.run(main())