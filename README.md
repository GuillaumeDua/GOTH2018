# GOTH2018

This is our participation to the [Game-Off 2018](https://itch.io/jam/game-off-2018)

## About the game :

> This is a **survival-management space game** that involve **evolution mecanisms**.<br>
> The game strongly rely on **procedural generation** to offer a **new experience at every try**.
>
> A **crew**, remains of the **planet Earth population** after the apocalypse,<br>
> is exploring the **space** in order to find a **suitable planet** to **live** on.<br>
> However, space is wide and **full of dangers**, and many things will happend.
>
> Will the crew finaly succeed ?<br>
> Will their still be so-called *"humans"* at the end, or **evolve** into something else ?
>
> Let's start **your very own journey** and see !

## Development stages :
- Main target OS is actually Windows 64 bits.
- Use [SFML](https://github.com/SFML/SFML) binaries ([DLL](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip))
- Use [ImGUI](https://github.com/ocornut/imgui) 
- Use [ImGUI-SFML](https://github.com/eliasdaler/imgui-sfml) for UI elements
- Use [GCL](https://github.com/GuillaumeDua/GCL_CPP)

## Technical details :
### Visual Studio solution :
- Use VS 2017 community
- VS Project's filters reflect FS
- SFML DLL added to repository for dev stages only *(for non-dev convinience)*
- Some project's pathes are hard-coded :
  - External libraries in "C:\Dev\Ext_libs" . e.g "C:\Dev\Ext_libs\SFML\SFML-2.5.0"
  - GCL in "C:\DEV\PROJECTS\cpp\GCL_CPP"

