-- Keep a log of any SQL queries you execute as you solve the mystery.

-- After stumbling out of bed and staring my day with the usual breakfast of stale whiskey and Maalox,
-- I started with a querie of crime_scene_reports to hopefully find a description that
-- matched the alleged duck-napping.
SELECT description FROM crime_scene_reports;

-- Luckily, 3 witnesses saw the duck get plucked at 10:15AM near the Chamberlin Street courthouse.
-- What did they have to say about this fowl deed?
SELECT transcript FROM interviews WHERE year = 2020 AND month = 07 AND day = 28;

-- Each witness provided some useful info: 1) the thief fled the courthouse by car no more than 10
-- min after the theft 2) the thief was recognized by a witness who saw him/her withdraw money from
-- an ATM by the courthouse earlier in the morning 3) the theif made a < 1 minute phone call to an
-- unknown party before leaving the courthouse, saying he/she would take the earliest flight tomorrow
-- and asking this accomplice to purchase the ticket


-- Let's see the owners of the vehicles that left the courthouse in our timeframe
-- who also withdrew money from the ATM on the day in question...
SELECT name FROM people 
    WHERE license_plate IN (SELECT license_plate FROM courthouse_security_logs 
    WHERE activity = 'exit' 
    AND year = 2020 AND month = 07 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25)
UNION ALL
SELECT name FROM people
    WHERE id IN (SELECT person_id FROM bank_accounts
    WHERE account_number IN (SELECT account_number FROM atm_transactions
    WHERE atm_location = 'Fifer Street' and transaction_type = 'withdraw'
    AND year = 2020 AND month = 07 AND day = 28))
ORDER BY name;

-- Danielle
-- Elizabeth
-- Ernest
-- Russel
-- Our perp is in that list!

-- Did any of them make a < 1 min phone call on that day? And to whom?!?
SELECT name FROM people
    WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2020 AND month = 07 AND day = 28 AND duration < 60);

-- Looks like Elizabeth and Danielle are off the hook...for now.
-- So, did Russel or Ernest take the earliest flight out of Dodge on the 29th?
SELECT id FROM flights 
    WHERE year = 2020 AND month = 07 AND day = 29 AND origin_airport_id = (SELECT id FROM airports WHERE city='Fiftyville') 
    ORDER BY hour, minute LIMIT 1;
    
-- This twisted scumbag was on flight 36!
SELECT city FROM airports WHERE id = (SELECT destination_airport_id FROM flights WHERE id = 36);
-- And fled to London!

-- Checking the names of the people on the flight...
SELECT name FROM people WHERE passport_number in (SELECT passport_number FROM passengers WHERE flight_id = 36);
-- we find Ernest.

-- And his collaborator...
SELECT name FROM people
    WHERE phone_number = (SELECT receiver FROM phone_calls
    WHERE caller = (SELECT phone_number FROM people 
    WHERE name = 'Ernest'
    AND year = 2020 AND month = 07 AND day = 28 AND duration < 60));
-- Berthold.

-- If I hadn't already lost all faith in anything good in this world I'd be puking my
-- damn guts out right now. But I've learned to stop trying to care. It's Fiftyville.
