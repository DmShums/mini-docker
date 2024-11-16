# mini-docker
```json
{
  "name": "mini-docker", // the name that will be used for CGroup
  "new_root": "/home/hellcaster/Desktop/mini-docker/tmp", // were will be the storage of current container
  "args": ["/vault_source/hello1"], // args to start a program (used in execv)
  "mount_points": ["/home/hellcaster/Desktop/mini-docker/binaries:/vault_source"], // mount <from>:<to>
  "copy_points": [""],
  "mem_hard_limit": 200,
  "mem_throttling_limit": 180,
  "swap_limit": 50,
  "pids_limit": 20
}
```

mount_points: <from>:<to>
- <from> - folder in filesystem
- <to> - desired location in mini-Docker filesystem

Warning: mount is not copying anything to Docker, it creates alias. This means that any change in any of those 2 folders immediatly cause changes in other folder. If you want copy files see `copy_points`

Also, there exist mount_points `"/lib:/lib"` (needs to be supplemented) by default.

copy_points: <from>:<to> copies folder and files recursively from <from> to <to>