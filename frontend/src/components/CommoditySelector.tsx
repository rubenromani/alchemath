import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '@/components/ui/select';
import { Commodity } from '@/types/analysis';

interface CommoditySelectorProps {
  commodities: Commodity[];
  value: string;
  onChange: (value: string) => void;
}

export const CommoditySelector = ({ commodities, value, onChange }: CommoditySelectorProps) => {
  return (
    <div className="space-y-2">
      <label className="text-sm font-medium text-slate-300">Symbol</label>
      <Select value={value} onValueChange={onChange}>
        <SelectTrigger className="bg-slate-700 border-slate-600 text-white">
          <SelectValue placeholder="Select commodity" />
        </SelectTrigger>
        <SelectContent className="bg-slate-700 border-slate-600">
          {commodities.map((commodity) => (
            <SelectItem 
              key={commodity.symbol} 
              value={commodity.symbol}
              className="text-white hover:bg-slate-600"
            >
              {commodity.symbol} - {commodity.name}
            </SelectItem>
          ))}
        </SelectContent>
      </Select>
    </div>
  );
};