/**
 * Header file for common utility functions. 
 * @author Assen Totin assen.totin@gmail.com
 * 
 * Created for the Voody Blue Subtitler suit, copyright (C) 2014 Assen Totin, assen.totin@gmail.com 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

void fix_new_line(char *);

long get_time_msec();

int convert_time_from_srt(char *, int);

void convert_time_to_srt(unsigned int, char *, int);

struct subtitle_srt *import_subtitles_srt(char *, int *, int *);

void get_locale_prefix(char *res);

void get_icon(char *res);

void get_file_selector_path(char *res);

void get_config_dir(char *res);

void get_dir_from_filename (char *filename, char *dir);

void del_old_logs();

struct vbs_stat_struct {
	char filename[1024];
	int unsigned mtime;
};

