# array with high times [start, end]
WEEK_SCHEDULE = [
    [datetime.time( 5, 0), datetime.time( 9, 0)], # from 05:00 to 09:00
    [datetime.time(18, 0), datetime.time(21, 00)] # from 18:00 to 21:00
]
WEEKEND_SCHEDULE = [
    [datetime.time( 5, 0), datetime.time(10, 0)],
    [datetime.time(18, 0), datetime.time(21, 00)]
]

TEMP_HIGH = 21.5
TEMP_LOW  = 20.0

climate_entity = 'climate.central_heating' # set to your thermostat entity
current_temp = hass.states.get(climate_entity).attributes['temperature']

now = datetime.datetime.now().time()
if datetime.datetime.now().date().weekday() < 5:
    current_schedule = WEEK_SCHEDULE
else:
    current_schedule = WEEKEND_SCHEDULE

in_high_time = False
for high_time in current_schedule:
    start = high_time[0]
    end = high_time[1]
    
    if start <= now <= end:        
        in_high_time = True
        break

new_temp = TEMP_HIGH if in_high_time else TEMP_LOW 
if new_temp != current_temp:
    # set the thermostat target temperature.
    hass.services.call('climate', 'set_temperature', {'entity_id': climate_entity, 'temperature': new_temp})
