-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local opening = import("app.core.gameplay.control.layout_base").create("opening")

local sprite    = import("app.core.graphical.sprite")
local fade      = import("app.core.graphical.fade")

function opening:onLoad()

    local initiate = cc.CallFunc:create(function()
                        self:opening_intro_a()
                     end)

    local sequence = cc.Sequence:create(initiate, nil)
    self:runAction(sequence)

    -- self variables
    self.triggered_ = false

    if cc.platform_ == "mobile" then
        self:setPositionX(85)
    end
end

function opening:opening_intro_a()
    self.greentwip_logo_ = sprite:create("sprites/gameplay/screens/opening/abakura/abakura_logo", cc.p(0, 0))
                                 :setPosition(cc.p(0,0))
                                 :addTo(self)

    local actions = {}
    actions[#actions + 1] = {name = "abakura_logo",   animation = {name = "abakura_logo",  forever = false, delay = 0.20} }

    self.greentwip_logo_:load_actions_set(actions, false)

    local pre_callback = cc.CallFunc:create(function()
        self.greentwip_logo_:run_action("abakura_logo")
    end)

    local duration = cc.DelayTime:create(self.greentwip_logo_:get_action_duration("abakura_logo"))
    local post_callback = cc.CallFunc:create(function()

        self:getApp():enterScene("screens.microsoft", "FADE", 1)

        
    end)

    local audio_callback = cc.CallFunc:create(function()
        cc.audio.play_sfx("sounds/sfx_abakura.mp3", false)       
    end)

    local sequence = cc.Sequence:create(pre_callback, duration, audio_callback, duration, post_callback, nil)

    self:runAction(sequence)
end

function opening:step(dt)
--    if not self.triggered_ then
--        if cc.key_pressed(cc.key_code_.start) then
--            self.triggered_ = true
--            cc.audio.play_sfx("sounds/sfx_selected.mp3")
--            self:getApp():enterScene("gameplay.stage_select", "FADE", 1)
--        end
--    end
--   self:getApp():enterScene("screens.testwrapper", "FADE", 1)

    self:post_step(dt)

    return self
end



return opening