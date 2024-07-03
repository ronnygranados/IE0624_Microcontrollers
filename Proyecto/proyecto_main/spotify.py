import spotipy
from spotipy.oauth2 import SpotifyOAuth

# Spotify credentials
client_id = 'b8f58d61b86d4d0fbbb6af9cafe21a4e'
client_secret = '7333222701e54a49b2bd579904ab58c6'
redirect_uri = 'http://localhost:8888/callback'

# Define the scope
scope = "user-modify-playback-state,user-read-playback-state,user-read-currently-playing"

# Authenticate with Spotify
sp = spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=client_id,
                                               client_secret=client_secret,
                                               redirect_uri=redirect_uri,
                                               scope=scope))
# Function to play music
def play_music():
    sp.start_playback()
    track_info = get_current_playing()
    if track_info:
        print(f"Now playing: {track_info['name']} by {', '.join(track_info['artists'])}")

# Function to pause music
def pause_music():
    sp.pause_playback()

# Function to get the currently playing track
def get_current_playing():
    current_track = sp.current_playback()
    if current_track and current_track['is_playing']:
        track = current_track['item']
        track_name = track['name']
        artists = [artist['name'] for artist in track['artists']]
        return {'name': track_name, 'artists': artists}
    else:
        print("No track is currently playing.")
        return None

# Main function to check the variable and control playback
def main():
    variable = 0  # Initial value of the variable

    while True:
        new_variable = int(input("Enter the variable value (0 or 1): "))
        
        if new_variable != variable:
            if new_variable == 1:
                play_music()
            elif new_variable == 0:
                pause_music()
            variable = new_variable

if __name__ == "__main__":
    main()