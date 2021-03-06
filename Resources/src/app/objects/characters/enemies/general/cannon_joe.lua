-- Copyright 2014-2015 Greentwip. All Rights Reserved.

local enemy   = import("app.objects.characters.enemies.base.enemy")
local mob     = class("cannon_joe", enemy)

function mob:onCreate()
    self.super:onCreate()
    self.default_health_ = 9
    self.power_ = 4
    self.attacking_ = false
    self.was_paused_ = false
    self.weapon_ = import("app.objects.weapons.enemies.general.directional_bullet")
    self.weapon_parameters_ = {
        category_ = "gameplay",
        sub_category_ = "level",
        package_ = "weapon",
        cname_ = "directional_bullet"
    }
end

function mob:animate(cname)

    local stand  =  { name = "stand", animation = { name = cname .. "_" .. "stand", forever = false, delay = 0.10} }
    local flip   =  { name = "flip",  animation = { name = cname .. "_" .. "flip",  forever = false, delay = 0.20} }
    local shoot  =  { name = "shoot", animation = { name = cname .. "_" .. "shoot", forever = false, delay = 0.04} }

    self.sprite_:load_action(stand, false)
    self.sprite_:load_action(flip, false)
    self.sprite_:load_action(shoot, false)
    self.sprite_:set_animation(cname .. "_" .. "stand")

    return self
end

function mob:onRespawn()
    self.attacking_ = false
    self.was_paused_ = false
    self.is_flipping_ = false
    self.status_ = cc.enemy_.status_.fighting_
    
    if self.sprite_:isFlippedX() then
        self.sprite_:setFlippedX(false)
    end
end

function mob:flip(x_normal)

    self.sprite_:run_action("flip")

    local delay = cc.DelayTime:create(self.sprite_:get_action_duration("flip"))
    local on_after_flip_delay = cc.DelayTime:create(self.sprite_:get_action_duration("flip") * 0.5)

    local callback = cc.CallFunc:create(function()
        self.sprite_:setFlippedX(true)
    end)

    if x_normal == -1 then  -- shall flip to left

        callback = cc.CallFunc:create(function()
            self.sprite_:setFlippedX(false)
        end)
    end

    local flip_callback = cc.CallFunc:create(function()
        self.is_flipping_ = false
        self.attacking_ = false
    end)

    local stand = cc.CallFunc:create(function()
        self.sprite_:run_action("stand")
    end)

    local sequence = cc.Sequence:create(delay, callback, on_after_flip_delay, flip_callback, stand, nil)

    self:stopAllActions()
    self:runAction(sequence)
end

function mob:attack()

    if cc.game_status_ == cc.GAME_STATUS.RUNNING then
        if self.was_paused_ then
            self.was_paused_ = false
            if self.attacking_ then
                local delay = cc.DelayTime:create(0.5)
                local cooldown = cc.CallFunc:create(function()
                    if cc.game_status_ == cc.GAME_STATUS.RUNNING then
                        self.attacking_ = false
                    end
                end)
    
                local sequence = cc.Sequence:create(delay, cooldown, nil)
    
                self:stopAllActions()
                self:runAction(sequence)
            end
        end
    else
        self.was_paused_ = true
    end

    if self.status_ == cc.enemy_.status_.fighting_ and not self.is_flipping_ and not self.attacking_ then

        local player_x_distance = cc.pGetDistance(cc.p(self:getPositionX(), 0), cc.p(self.player_:getPositionX(), 0))
        local player_y_distance = cc.pGetDistance(cc.p(0, self:getPositionY()), cc.p(0, self.player_:getPositionY()))

        if player_x_distance > 48 or player_y_distance <= 24 then

            self.attacking_ = true


            local delay = cc.DelayTime:create(0.5)
            local callback = cc.CallFunc:create(function()

            player_x_distance = cc.pGetDistance(cc.p(self:getPositionX(), 0), cc.p(self.player_:getPositionX(), 0))
            player_y_distance = cc.pGetDistance(cc.p(0, self:getPositionY()), cc.p(0, self.player_:getPositionY()))

                if player_x_distance > 48 or player_y_distance <= 24 then

                    local player_position = cc.p(self.player_:getPositionX(), self.player_:getPositionY())

                    local bullet = self:fire({  sfx = nil,
                                                offset = cc.p(20, 16),
                                                weapon = self.weapon_,
                                                parameters = self.weapon_parameters_})
                    
                    if player_x_distance < 30 and player_y_distance < 16 then
                        local x_normal = 0
                        if self.sprite_:isFlippedX() then
                            x_normal = 1
                        else
                            x_normal = -1
                        end

                        bullet:fire_straight(x_normal)
                    else
                        bullet:setup_movement(player_position)
                    end
                end

                

            end)
            
            local cooldown = cc.CallFunc:create(function()
                if cc.game_status_ == cc.GAME_STATUS.RUNNING then
                    self.attacking_ = false
                end
            end)

            local sequence = cc.Sequence:create(delay, callback, delay, cooldown, nil)

            self:stopAllActions()
            self:runAction(sequence)
        end
    end

end

return mob

