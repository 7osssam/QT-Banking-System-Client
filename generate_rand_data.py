import random
import re
import psycopg2
from faker import Faker
from datetime import datetime

# Predefined list of common Arabic Egyptian names
first_names = [
    'Hossam', 'Ahmed', 'Youssef', 'Omar', 'Ali', 'Mahmoud', 'Amr', 'Khaled', 'Mostafa', 'Mohamed',
    'Sara', 'Mona', 'Yasmin', 'Fatma', 'Noor', 'Layla', 'Nadine', 'Hana', 'Reem', 'Amina'
]

last_names = [
    'Hassan', 'Mohamed', 'Ali', 'Ibrahim', 'Sayed', 'Mahmoud', 'Mostafa', 'Hussein', 'Salem', 'Fathy'
]

# Database connection settings
db_settings = {
    'host': 'aws-0-us-east-1.pooler.supabase.com',
    'port': 6543,
    #'user': 'postgres.ajfrcwmgtitxbvcnhzwn', # Production
    'user': 'postgres.yflugnxlcmfwjczwzspw', # Testing
    #'password': 'z7HQs^*#HnmqQ3m2', # Production
    'password': 'dpd2q2*5BULBCz$J', # Testing
    'dbname': 'postgres'
}

# Function to generate a valid password
def generate_password():
    while True:
        password = fake.password(length=8, special_chars=False)
        if (re.search(r'\d', password) and
                re.search(r'[a-z]', password) and
                re.search(r'[A-Z]', password)):
            return password

# Function to generate a random 6-digit account number
def generate_account_number():
    return random.randint(100000, 999999)

# Initialize Faker for generating email addresses
fake = Faker()

# Generate users, accounts, and transactions data
users = []
accounts = []
transactions = []

# Generate 2 admin users
for _ in range(2):
    first_name = random.choice(first_names)
    last_name = random.choice(last_names)
    email = fake.email()
    password = generate_password()
    users.append((first_name, last_name, email, password, 'admin'))

# Generate 8 normal users and their accounts
for _ in range(8):
    first_name = random.choice(first_names)
    last_name = random.choice(last_names)
    email = fake.email()
    password = generate_password()
    users.append((first_name, last_name, email, password, 'user'))
    account_number = generate_account_number()
    balance = round(random.uniform(1000, 10000), 2)
    accounts.append((account_number, len(users), balance))

# Generate fake transactions between user accounts
for _ in range(20):  # Generating 20 transactions
    from_account = random.choice(accounts)
    to_account = random.choice(accounts)
    while to_account == from_account:
        to_account = random.choice(accounts)
    amount = round(random.uniform(10, 1000), 2)
    transactions.append((from_account[0], from_account[0], to_account[0], amount, datetime.now(), datetime.now()))

# Connect to the PostgreSQL database
conn = psycopg2.connect(
    host=db_settings['host'],
    port=db_settings['port'],
    user=db_settings['user'],
    password=db_settings['password'],
    dbname=db_settings['dbname']
)
cur = conn.cursor()

# Insert users into the Users table
cur.execute("BEGIN;")
try:
    user_ids = []
    for user in users:
        cur.execute(
            "INSERT INTO Users (first_name, last_name, email, password, role, created_at, updated_at) "
            "VALUES (%s, %s, %s, %s, %s, %s, %s) RETURNING id;",
            (user[0], user[1], user[2], user[3], user[4], datetime.now(), datetime.now())
        )
        user_id = cur.fetchone()[0]
        user_ids.append(user_id)

    # Insert accounts into the Accounts table
    for i, account in enumerate(accounts):
        cur.execute(
            "INSERT INTO Accounts (account_number, user_id, balance, created_at) "
            "VALUES (%s, %s, %s, %s);",
            (account[0], user_ids[account[1] - 1], account[2], datetime.now())
        )

    # Insert transactions into the Transactions table
    for transaction in transactions:
        cur.execute(
            "INSERT INTO Transactions (account_number, from_account_number, to_account_number, amount, created_at, updated_at) "
            "VALUES (%s, %s, %s, %s, %s, %s);",
            transaction
        )
    conn.commit()
except Exception as e:
    print(f"Error: {e}")
    conn.rollback()
finally:
    cur.close()
    conn.close()

print("Data insertion completed successfully.")
