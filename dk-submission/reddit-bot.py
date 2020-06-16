import praw

# pass authentication information from the local praw.ini file
reddit = praw.Reddit("bot1")

# used to define which subreddit will be iterated through
subreddit = reddit.subreddit("all")

# defines the number of posts iterated through
limit = 10

scrabble_points= {"a": 1, "b": 3, "c": 3,"d": 2, "e": 1, "f": 4, "g": 2, "h": 4,
				  "i": 1, "j": 8, "k": 5, "l": 1, "m": 3, "n": 1, "o": 1,
				  "p": 3, "q": 10, "r": 1, "s": 1, "t": 1, "u": 1, "v": 4,
				  "w": 4, "x": 8, "y": 4, "z": 10}

# returns the defined number of posts on new as a list
def get_title():
	return [submission.title for submission in subreddit.new(limit=limit)]

# iterates through individual letters of post titles and scores them according
# to the points assigned in scrabble
def score_title(title):
	# seperates titles into individual words
	for word in title:
		total = 0
		# seperates words into individual letters
		for letter in word:
			# turns all letters into lowercase
			letter = letter.lower()
			# checks to see if letter is actually a letter (numbers/symbols/spaces/etc are skipped)
			if letter in scrabble_points:
				total += scrabble_points[letter]
		print(word + "\nPOINT TOTAL =",total,"\n")

score_title(get_title())