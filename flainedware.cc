if not isfolder("flainedware") then 
    makefolder("flainedware")
end
local Settings = {
    boxesp = false,
    nameesp = false,
    tracers = false,
    espcolorline = Color3.fromRGB(255,255,255),
    espcolorname = Color3.fromRGB(255,255,255),
    espcolorbox = Color3.fromRGB(255,255,255),
    weaponesp = false,
    outlineesp = false,
    espenabled = false,
    healthesp = false,
    chams = false
}

local players = game:GetService("Players")
local camera = workspace.CurrentCamera
local gs = game:GetService("GuiService"):GetGuiInset()
local ESPMain = {}
local straylines = false
local fov = game.workspace.CurrentCamera.FieldOfView

local cursor = Drawing.new("Circle")
cursor.Visible = true
cursor.Transparency = 1 
cursor.Color = Color3.fromRGB(255,255,255)
cursor.Filled = true
cursor.Radius = 5

createline = function()
    local line = Drawing.new("Line")
    line.Thickness = 1
    line.Transparency = 1
    line.Visible = true
    line.Color = Settings.espcolorline
    line.From = Vector2.new(camera.ViewportSize.X / 2, camera.ViewportSize.Y / 2 + 400)
    return line
end
 
createlabel = function(text)
    local label = Drawing.new("Text")
    label.Transparency = 1
    label.Visible = true
    label.Color = Settings.espcolorname
    label.Center = true
    label.Outline = true
    label.Text = text
    label.Size = 15
    return label
end
 
createbox = function()
    local box = Drawing.new("Square")
    box.Transparency = 1
    box.Thickness = 1
    box.Visible = true
    box.Color = Settings.espcolorbox
    return box
end
 
local player = players.LocalPlayer
function isInTeam(char)
    if teamcheck then
        if player and players:GetPlayerFromCharacter(char) and players:GetPlayerFromCharacter(char).Team and player.Team then
            if players:GetPlayerFromCharacter(char).Team ~= player.Team then
                return false
            else
                return (player.Team == players:GetPlayerFromCharacter(char).Team)
            end
        end
    else
        return false
    end
end
 
game:GetService("RunService").Stepped:Connect(
    function()
        cursor.Position = Vector2.new(game.Players.LocalPlayer:GetMouse().X, game.Players.LocalPlayer:GetMouse().Y + gs.Y)
        fov = game.workspace.CurrentCamera.FieldOfView
        spawn(
            function()
                for i, v in pairs((game:GetService("Players")):GetChildren()) do
                    if      v.Name ~= game:GetService("Players").LocalPlayer.Name and
                            v.Name ~= game.Players.LocalPlayer.Name and
                            v.Character and
                            v.Character:FindFirstChild("HumanoidRootPart") and
                            v.Character:FindFirstChild("LowerTorso") and
                            (not isInTeam(v.Character)) and
                            v.Character:FindFirstChild("Head") and
                            v.Character:FindFirstChild("Humanoid") and
                            Settings.espenabled
                    then
                        if not ESPMain[v.Name] then
                            ESPMain[v.Name] = {
                                v.Character
                            }
                        end
                        local vector, onScreen = camera:WorldToScreenPoint(v.Character["UpperTorso"].Position - Vector3.new(0, 0.5, 0))
                        local vectorhead, onScreenhead = camera:WorldToScreenPoint(v.Character["Head"].Position + Vector3.new(0, 1, 0))
                        local vectorfeet, onScreenfeet = camera:WorldToScreenPoint(v.Character["HumanoidRootPart"].Position - Vector3.new(0, 3.55, 0))
                        local vectortwo = vectorhead.Y + ((vectorfeet.Y - vectorhead.Y) / 2)
                        if Settings.nameesp or Settings.healthesp then
                            if ESPMain[v.Name][4] then
                                ESPMain[v.Name][4].Visible = (onScreen and Settings.nameesp)
                                ESPMain[v.Name][4].Position = Vector2.new(vectorhead.X, vectorhead.Y + (gs.Y / 2))
                                if Settings.nameesp then 
                                    ESPMain[v.Name][4].Text = v.Name
                                end
                                if Settings.healthesp then
                                    ESPMain[v.Name][4].Text = math.floor(v.Character.Humanoid.Health) .. "/" .. v.Character.Humanoid.MaxHealth
                                end
                                if Settings.healthesp and Settings.nameesp then 
                                    ESPMain[v.Name][4].Text = v.Name .. " | " .. math.floor(v.Character.Humanoid.Health) .. "/" .. v.Character.Humanoid.MaxHealth
                                end
                                
                                ESPMain[v.Name][4].Color = Settings.espcolorname
                            else
                                ESPMain[v.Name][4] = createlabel(v.Name)
                                ESPMain[v.Name][4].Visible = (onScreen and Settings.nameesp)
                                ESPMain[v.Name][4].Position = Vector2.new(vectorhead.X, vectorhead.Y + (gs.Y / 2))
                                
                                ESPMain[v.Name][4].Color = Settings.espcolorname
                            end
                        else
                            if ESPMain[v.Name] then
                                if ESPMain[v.Name][4] then
                                    ESPMain[v.Name][4]:Remove()
                                    ESPMain[v.Name][4] = nil
                                end
                            end
                        end 
                        if Settings.weaponesp then 
                            if ESPMain[v.Name][6] then
                                ESPMain[v.Name][6].Visible = (onScreen and Settings.weaponesp)
                                ESPMain[v.Name][6].Position = Vector2.new(vectorfeet.X, vectorfeet.Y + (gs.Y / 2) + 20)
                                ESPMain[v.Name][6].Text = v.Character.EquippedTool.Value
                                ESPMain[v.Name][6].Color = Settings.espcolorname
                            else
                                ESPMain[v.Name][6] = createlabel(v.Name)
                                ESPMain[v.Name][6].Visible = (onScreen and Settings.weaponesp)
                                ESPMain[v.Name][6].Position = Vector2.new(vectorhead.X, vectorhead.Y + (gs.Y / 2))
                                
                                ESPMain[v.Name][6].Color = Settings.espcolorname
                            end
                        else
                            if ESPMain[v.Name] then
                                if ESPMain[v.Name][6] then
                                    ESPMain[v.Name][6]:Remove()
                                    ESPMain[v.Name][6] = nil
                                end
                            end
                        end
                        if straylines then
                            local calc = vectorhead.y - vectorfeet.y
                            if ESPMain[v.Name][5] then
                                ESPMain[v.Name][5].Visible = false
                                ESPMain[v.Name][5].Color = Settings.espcolorline
                                ESPMain[v.Name][5].To = Vector2.new(vector.X, vector.Y + gs.Y - calc / 2)
                                ESPMain[v.Name][5].From =
                                    Vector2.new(camera.ViewportSize.X / 2, camera.ViewportSize.Y / 2 + 400)
                            else
                                ESPMain[v.Name][5] = createline()
                                ESPMain[v.Name][5].Visible = false
                                ESPMain[v.Name][5].To = Vector2.new(vector.X, vectortwo + gs.Y - calc / 2)
                                ESPMain[v.Name][5].From =
                                    Vector2.new(camera.ViewportSize.X / 2, camera.ViewportSize.Y / 2 + 400)
                                ESPMain[v.Name][5].Color = Settings.espcolorline
                            end
                        else
                            if ESPMain[v.Name][5] then
                                ESPMain[v.Name][5]:Remove()
                                ESPMain[v.Name][5] = nil
                            end
                        end
                        if Settings.tracers then
                            local calc = vectorhead.y - vectorfeet.y
                            if ESPMain[v.Name][2] then
                                ESPMain[v.Name][2].Visible = (onScreen and Settings.tracers)
                                ESPMain[v.Name][2].Color = Settings.espcolorline
                                ESPMain[v.Name][2].To = Vector2.new(vector.X, vectortwo + gs.Y - calc / 2)
                                ESPMain[v.Name][2].From =
                                    Vector2.new(camera.ViewportSize.X / 2, camera.ViewportSize.Y / 2 + 400)
                            else
                                ESPMain[v.Name][2] = createline()
                                ESPMain[v.Name][2].Visible = (onScreen and Settings.tracers)
                                ESPMain[v.Name][2].To = Vector2.new(vector.X, vector.Y + gs.Y - calc / 2)
                                ESPMain[v.Name][2].From =
                                    Vector2.new(camera.ViewportSize.X / 2, camera.ViewportSize.Y / 2 + 400)
                                ESPMain[v.Name][2].Color = Settings.espcolorline
                            end
                        else
                            if ESPMain[v.Name][2] then
                                ESPMain[v.Name][2]:Remove()
                                ESPMain[v.Name][2] = nil
                            end
                        end
                        if Settings.outlineesp then
                            local calc = vectorhead.y - vectorfeet.y
                            if ESPMain[v.Name][7] then
                                ESPMain[v.Name][7].Visible = (onScreen and Settings.outlineesp)
                                ESPMain[v.Name][7].Size = Vector2.new((1400 / vector.Z)*(135/fov), calc)
                                ESPMain[v.Name][7].Color = Color3.fromRGB(0,0,0)
                                ESPMain[v.Name][7].Thickness = 3
                                ESPMain[v.Name][7].Position =
                                    Vector2.new(
                                    vector.X - ESPMain[v.Name][7].Size.X / 2,
                                    vectortwo + game:GetService("GuiService"):GetGuiInset().Y -
                                        ESPMain[v.Name][7].Size.Y / 2
                                )
                            else
                                ESPMain[v.Name][7] = createbox()
                                ESPMain[v.Name][7].Visible = (onScreen and Settings.outlineesp)
                                ESPMain[v.Name][7].Size = Vector2.new(1400 / vector.Z, calc)
                                ESPMain[v.Name][7].Color = Color3.fromRGB(0,0,0)
                                ESPMain[v.Name][7].Thickness = 3
                                ESPMain[v.Name][7].Position =
                                    Vector2.new(
                                    vector.X - ESPMain[v.Name][7].Size.X / 2,
                                    vector.Y + game:GetService("GuiService"):GetGuiInset().Y -
                                        ESPMain[v.Name][7].Size.Y / 2
                                )
                            end
                        else
                            if ESPMain[v.Name][7] then
                                ESPMain[v.Name][7]:Remove()
                                ESPMain[v.Name][7] = nil
                            end
                        end
                        if Settings.boxesp then
                            local calc = vectorhead.y - vectorfeet.y
                            if ESPMain[v.Name][3] then
                                ESPMain[v.Name][3].Visible = (onScreen and Settings.boxesp)
                                ESPMain[v.Name][3].Size = Vector2.new((1400 / vector.Z)*(135/fov), calc)
                                ESPMain[v.Name][3].Color = Settings.espcolorbox
                                ESPMain[v.Name][3].Position =
                                    Vector2.new(
                                    vector.X - ESPMain[v.Name][3].Size.X / 2,
                                    vectortwo + game:GetService("GuiService"):GetGuiInset().Y -
                                        ESPMain[v.Name][3].Size.Y / 2
                                )
                            else
                                ESPMain[v.Name][3] = createbox()
                                ESPMain[v.Name][3].Visible = (onScreen and Settings.boxesp)
                                ESPMain[v.Name][3].Size = Vector2.new(1400 / vector.Z, calc)
                                ESPMain[v.Name][3].Color = Settings.espcolorbox
                                ESPMain[v.Name][3].Position =
                                    Vector2.new(
                                    vector.X - ESPMain[v.Name][3].Size.X / 2,
                                    vector.Y + game:GetService("GuiService"):GetGuiInset().Y -
                                        ESPMain[v.Name][3].Size.Y / 2
                                )
                            end
                        else
                            if ESPMain[v.Name][3] then
                                ESPMain[v.Name][3]:Remove()
                                ESPMain[v.Name][3] = nil
                            end
                        end
                        
                    else
                        if ESPMain[v.Name] then
                            if ESPMain[v.Name][2] then
                                ESPMain[v.Name][2]:Remove()
                                ESPMain[v.Name][2] = nil
                            end
                            if ESPMain[v.Name][3] then
                                ESPMain[v.Name][3]:Remove()
                                ESPMain[v.Name][3] = nil
                            end
                            if ESPMain[v.Name][4] then
                                ESPMain[v.Name][4]:Remove()
                                ESPMain[v.Name][4] = nil
                            end
                            if ESPMain[v.Name][6] then
                                ESPMain[v.Name][6]:Remove()
                                ESPMain[v.Name][6] = nil
                            end
                            if ESPMain[v.Name][7] then
                                ESPMain[v.Name][7]:Remove()
                                ESPMain[v.Name][7] = nil
                            end
                        end
                    end
                end
            end
        )
    end
)
 
players.PlayerRemoving:Connect(
    function(plr)
        if ESPMain[plr.Name] then
            if ESPMain[plr.Name][2] then
                ESPMain[plr.Name][2]:Remove()
                ESPMain[plr.Name][2] = nil
            end
            if ESPMain[plr.Name][3] then
                ESPMain[plr.Name][3]:Remove()
                ESPMain[plr.Name][3] = nil
            end
            if ESPMain[plr.Name][4] then
                ESPMain[plr.Name][4]:Remove()
                ESPMain[plr.Name][4] = nil
            end
            ESPMain[plr.Name] = nil
        end
    end
)
Settings.chams = false 

function esp()
    local p = game.Players:GetPlayers()
    
    for i,v in pairs(game.Players.LocalPlayer.PlayerGui:GetChildren()) do
        if v:IsA('Folder') and (game:GetService('Players'):FindFirstChild(v.Name) or v:FindFirstChildOfClass("BoxHandleAdornment")) then
            v:Destroy()
        end
    end
    if Settings.chams == false then
        return false
    end
    
    for i,v in pairs(p) do
        if workspace:FindFirstChild(v.Name) and (v.Character:FindFirstChild('Head')) then
            if v ~= game.Players.LocalPlayer and v.Character:FindFirstChild("Humanoid") then
                local folder = Instance.new('Folder', game.Players.LocalPlayer.PlayerGui)
                folder.Name = v.Name
                if v.Character.Humanoid.Health > 0 and Settings.chams then
                    for _,part in pairs(v.Character:GetChildren()) do
                        if part:IsA('BasePart') and not isInTeam(v.Character) then
                            local adorn = Instance.new('BoxHandleAdornment',folder)
                            adorn.Name = v.Name
                            if part.Name ~= "Head" then
                                adorn.Size = part.Size
                            else
                                adorn.Size = (part.Size)-Vector3.new(part.Size.X/4,0,0)
                            end
                            adorn.Adornee = part
                            adorn.AlwaysOnTop = true
                            adorn.ZIndex = 5
                            adorn.Transparency = 1 / 100
                            local c = Color3.fromRGB(255, 255, 255)
                            adorn.Color3 = c 
                            local adorn2 = Instance.new('BoxHandleAdornment',folder)
                            adorn2.Name = v.Name 
                            if part.Name ~= "Head" then
                                adorn2.Size = part.Size*1.1
                            else
                                adorn2.Size = (part.Size*1.1)-Vector3.new(part.Size.X/4,0,0)
                            end
                        end
                    end
                end
            end
        end
    end
    
    return true 
end
local function addColors(c1,c2)
    return Color3.new(c1.R+c2.R,c1.G+c2.G,c1.B+c2.B)
end

Settings.espcolorbox = Color3.fromRGB(255,255,255)
Settings.espcolorline = Color3.fromRGB(255, 255, 255)
Settings.espcolorname = Color3.fromRGB(255, 255, 255)
straylines = true

------------------------------------------

local Finity = loadstring(game:HttpGet("https://raw.githubusercontent.com/bloodball/UI-Librarys/main/Finity%20UI%20Lib"))()

local FinityWindow = Finity.new(true)
FinityWindow.ChangeToggleKey(Enum.KeyCode.O)
Finity.settitle("flainedware.cc")

local LegitCategory = FinityWindow:Category("Legit")
local RageCategory = FinityWindow:Category("Rage")
local VisualsCategory = FinityWindow:Category("Visuals")
local MiscCategory = FinityWindow:Category("Miscellaneous")
local SettingsCategory = FinityWindow:Category("Settings")

--LEGIT--
local LegitBot = LegitCategory:Sector("Legitbot")
local LegitTriggerBot = LegitCategory:Sector("Triggerbot")

--RAGE--
local RageBot = RageCategory:Sector("Main")
local RageAntiAim = RageCategory:Sector("Anti-Aim")
local RageOther = RageCategory:Sector("Other")

--VISUALS--
local VisualsPlayerESP = VisualsCategory:Sector("Player ESP")
local VisualsDroppedESP = VisualsCategory:Sector("Dropped ESP")
local VisualsGrenadeESP = VisualsCategory:Sector("Grenade ESP")
local VisualsWorld = VisualsCategory:Sector("Viewmodel")

VisualsPlayerESP:Cheat("Checkbox", "Enabled", function(val)
    Settings.espenabled = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Box ESP", function(val)
    Settings.boxesp = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Box outline", function(val)
    temp = Settings.boxesp 
    Settings.boxesp = not temp
    Settings.outlineesp = val
    wait()
    Settings.boxesp = temp
end)
VisualsPlayerESP:Cheat("Checkbox", "Tracer ESP", function(val)
    Settings.tracers = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Name ESP", function(val)
    Settings.nameesp = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Health ESP", function(val)
    Settings.healthesp = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Weapon ESP", function(val)
    Settings.weaponesp = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Teamcheck", function(val)
    teamcheck = val 
end)
VisualsPlayerESP:Cheat("Checkbox", "Chams ESP", function(val)
    Settings.chams = val 
end)

--MISC--

--SETTINGS--
local function Save(Name)
    if not isfile("flainedware/"..Name..".cfg") then return end 
    writefile("flainedware/"..Name..".cfg", game:service'HttpService':JSONEncode(Settings))
end
local function CreateCFG(Name)
    if isfile("flainedware/"..Name..".cfg") then return end 
    writefile("flainedware/"..Name..".cfg", game:service'HttpService':JSONEncode(Settings))
end
local function Load(Name)
    if not isfile("flainedware/"..Name..".cfg") then return end
    Settings = game:service'HttpService':JSONDecode(readfile("flainedware/"..Name..".cfg"))
end
local CFGNAME = ""
local SettingsConfigs = SettingsCategory:Sector("Configs")
SettingsConfigs:Cheat("Textbox", "CFG Name", function(val)
    CFGNAME = val
end)
SettingsConfigs:Cheat("Button", "Create", function()
    CreateCFG(CFGNAME)
end)
SettingsConfigs:Cheat("Button", "Save", function()
    Save(CFGNAME)
end)
SettingsConfigs:Cheat("Button", "Load", function()
    Load(CFGNAME)
end)

--CHAMS and OTHERS--
local UIS = game:GetService("UserInputService")
UIS.InputBegan:Connect(function(input, gameProcessed)
    if input.KeyCode then 
        if input.KeyCode == Enum.KeyCode.O then 
            cursor.Visible = not cursor.Visible
        end
    end
end)
while true do 
    esp()
    wait()
end
