import moteus
import asyncio

async def main():
    c = moteus.Controller()
    await c.set_stop()

    result = await c.set_position(position=0, accel_limit=5.0)
    print(result)

    #while True:
        #for position in [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]:
            #result = await c.set_position(position=position, accel_limit=5.0)
            #print(result)


if __name__=="__main__":
    asyncio.run(main())